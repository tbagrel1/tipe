#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Optimizes escape_probs params for bitwise_ppm_exe."""

import os
import random

ORDER = 3
EXE = "./bitwise_ppm_exe_{}".format(ORDER)
INIT_PROBS = ([0.0, 0.0001, 0.001, 0.1])[:ORDER + 1]
RAW_FILE = "calgary_{}.tar".format(ORDER)
ENC_FILE = "calgary_{}.enc".format(ORDER)
DEC_FILE = "calgary_{}.dec".format(ORDER)
JUNK_FILE = "junk.log"
RAW_SIZE = os.path.getsize(RAW_FILE)
ENC_CMD_PARTIAL = "{} enc {} {} {{}} > {}".format(
    EXE, RAW_FILE, ENC_FILE, JUNK_FILE)
DEC_CMD_PARTIAL = "{} dec {} {} {{}} > {}".format(
    EXE, ENC_FILE, DEC_FILE, JUNK_FILE)
CHECK_CMD = "cmp -s {} {}".format(RAW_FILE, DEC_FILE)
MIN_PROB = 1e-9
MAX_PROB = 1 - 1e-9

MUTATE_PROB_CHANCE = 0.25
MUTATE_COMP_CHANCE = 0.5
MIN_STRENGTH = -7
MAX_STRENGTH = -1
POPULATION_SIZE = 25
ITERATIONS = 25


def enc_cmd(probs):
    """Returns enc command corresponding to the specified probs."""
    return ENC_CMD_PARTIAL.format(" ".join((str(p) for p in probs)))


def dec_cmd(probs):
    """Returns dec command corresponding to the specified probs."""
    return DEC_CMD_PARTIAL.format(" ".join((str(p) for p in probs)))


def fitness(probs):
    """Returns the fitness of the specified probs."""
    if (os.system(enc_cmd(probs)) != 0 or
            os.system(dec_cmd(probs)) != 0 or
            os.system(CHECK_CMD) != 0):
        print("Error with <{}>".format(probs))
        return float("inf")
    else:
        return os.path.getsize(ENC_FILE) / RAW_SIZE


def mutate(probs):
    """Mutates the specified probs."""
    for i in range(1, ORDER + 1):
        if random.random() <= MUTATE_COMP_CHANCE:
            strength = random.randint(MIN_STRENGTH, MAX_STRENGTH)
            new_prob = probs[i] + (2 * random.random() - 1) * 10 ** strength
            if new_prob > MAX_PROB:
                new_prob = MAX_PROB
            if new_prob < MIN_PROB:
                new_prob = MIN_PROB
            probs[i] = new_prob


def mix(probs_a, probs_b):
    """Returns the child of probs_a and probs_b (average)."""
    probs_c = [0.0]
    for i in range(1, ORDER + 1):
        probs_c.append((probs_a[i] + probs_b[i]) / 2)
    return probs_c


def iterate(i, population):
    """Makes an iteration on the specified population."""
    print("%.3f %% done" % (i / ITERATIONS * 100))
    for i in range(POPULATION_SIZE):
        if random.random() <= MUTATE_PROB_CHANCE:
            mutate(population[i][1])
            population[i][0] = fitness(population[i][1])
    for i in range(POPULATION_SIZE // 2):
        new_probs = mix(population[2 * i][1], population[2 * i + 1][1])
        population.append([fitness(new_probs), new_probs])
    population.sort(key=lambda x: x[0])
    del population[POPULATION_SIZE:]


def main():
    """Launcher."""
    init_fitness = fitness(INIT_PROBS)
    population = []
    for k in range(POPULATION_SIZE):
        population.append([init_fitness, INIT_PROBS[:]])
    for i in range(ITERATIONS):
        iterate(i, population)
    print("100.000 % done")
    print(population[0])


if __name__ == "__main__":
    main()
