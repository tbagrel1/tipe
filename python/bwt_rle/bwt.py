#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Naive Burrows-Wheeler transform implementation."""

BLOCK_SIZE = 512
SPECIAL_CHAR = "@"


def padding_spliting(string):
    """Adds special non-meaningful symbols at the end of the string to match
    a multiple of BLOCK_SIZE and splits the string into BLOCK_SIZE chars
    blocks."""
    string += SPECIAL_CHAR * (BLOCK_SIZE - len(string) % BLOCK_SIZE)
    parts = []
    for i in range(len(string) // BLOCK_SIZE):
        parts.append(string[i * BLOCK_SIZE:(i + 1) * BLOCK_SIZE])
    return parts


def process_block(block):
    """Computes BWT on the specified BLOCK_SIZE chars block."""
    rotation_tab = []
    for i in range(BLOCK_SIZE):
        rotation_tab.append(
            (block[BLOCK_SIZE - i:] + block[:BLOCK_SIZE - i], i))
    rotation_tab.sort(key=lambda elt: elt[0])
    original_index = -1
    for (i, (s, k)) in enumerate(rotation_tab):
        if k == 0:
            original_index = i
            break
    return original_index, "".join([s[-1] for (s, k) in rotation_tab])


def unprocess_data(original_index_data):
    """Computes inverse BWT on the specified BLOCK_SIZE chars block."""
    (original_index, data) = original_index_data
    construction_tab = []
    for _ in range(BLOCK_SIZE):
        construction_tab.append("")
    for _ in range(BLOCK_SIZE):
        for i in range(BLOCK_SIZE):
            construction_tab[i] = data[i] + construction_tab[i]
        construction_tab.sort()
    return construction_tab[original_index]


def bwt(string):
    """Computes the BWT on the specified string and returns a list of
    mutated blocks."""
    return [process_block(block) for block in padding_spliting(string)]


def inverse_bwt(mutated_data_set):
    """Computes the inverse BWT on the specified data set and returns a string
    corresponding to the initial string."""
    padded_original_string = "".join([unprocess_data(data)
                                      for data in mutated_data_set])
    i = len(padded_original_string)
    while i > 0 and padded_original_string[i - 1] == SPECIAL_CHAR:
        i -= 1
    return padded_original_string[:i]


def main():
    """Launcher."""
    my_test_string = "Je suis ton pere !"
    print(inverse_bwt(bwt(my_test_string)))

if __name__ == "__main__":
    main()
