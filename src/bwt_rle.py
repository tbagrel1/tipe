#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Test with BTW + RLE."""

import math

import bwt
import rle
import sample


def encode(text):
    """Encodes the specified text with RLE o BTW."""
    data_set = bwt.bwt(text)
    encoded_set = []
    for (original_index, data) in data_set:
        encoded_set.append((original_index, rle.encode(data)))
    return encoded_set


def decode(encoded_set):
    """Decodes the specified data set with inv_RLE o inv_BWT."""
    mutated_data_set = []
    for (original_index, encoded_data) in encoded_set:
        mutated_data_set.append((original_index, rle.decode(encoded_data)))
    return bwt.inverse_bwt(mutated_data_set)


def efficiency(text):
    """Computes compression rate obtained using BWT and RLE."""
    n = len(text)
    encoded_set = encode(text)
    bits = 0
    for (original_index, encoded_data) in encoded_set:
        bits += int(math.ceil(math.log(bwt.BLOCK_SIZE, 2)))
        bits += (rle.N_NB + rle.N_CHARS) * len(encoded_data)
    return (1 - (bits / (7 * n))) * 100


# Computes efficiency using rle.N_NB + rle.N_CHARS and the returned encoded_set

def main():
    """Launcher."""
    print(efficiency(sample.SAMPLE))


if __name__ == "__main__":
    main()
