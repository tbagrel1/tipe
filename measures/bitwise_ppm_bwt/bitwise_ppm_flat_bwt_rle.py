#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Bench bitwise_ppm_flat + BWT + RLE."""

import os
import json

MAX_CONTEXT_SIZE = 28
MAX_COUNT_BITS = 8
PREFIX = "calgary_flat_bwt_rle"
EXE1 = "./bitwise_ppm_flat_exe"
EXE2 = "./rle_exe"
RAW_FILE = PREFIX + ".tar"
ENC1_FILE = PREFIX + ".enc1"
BWT_FILE = PREFIX + ".bwt"
ENC2_FILE = PREFIX + ".enc2"
DEC2_FILE = PREFIX + ".dec2"
UNBWT_FILE = PREFIX + ".unbwt"
DEC1_FILE = PREFIX + ".dec1"
ZIP_FILE = PREFIX + ".zip"
JUNK_FILE = "junk.log"
RAW_SIZE = os.path.getsize(RAW_FILE)
ZIP_RATIO = os.path.getsize(ZIP_FILE) / RAW_SIZE
ENC1_CMD_PARTIAL = "{} enc {} {} {{}} > {}".format(
    EXE1, RAW_FILE, ENC1_FILE, JUNK_FILE)
DEC1_CMD_PARTIAL = "{} dec {} {} {{}} > {}".format(
    EXE1, UNBWT_FILE, DEC1_FILE, JUNK_FILE)
BWT_CMD = "./bwt {} {} > {}".format(
    ENC1_FILE, BWT_FILE, JUNK_FILE)
UNBWT_CMD = "./unbwt {} {} > {}".format(
    DEC2_FILE, UNBWT_FILE, JUNK_FILE)
ENC2_CMD_PARTIAL = "{} enc {} {} {{}} > {}".format(
    EXE2, BWT_FILE, ENC2_FILE, JUNK_FILE)
DEC2_CMD_PARTIAL = "{} dec {} {} {{}} > {}".format(
    EXE2, ENC2_FILE, DEC2_FILE, JUNK_FILE)
CHECK_CMD = "cmp -s {} {}".format(RAW_FILE, DEC1_FILE)

def main():
    """Launcher."""
    results = {}
    for context_size in range(MAX_CONTEXT_SIZE + 1):
        for count_bits in range(1, MAX_COUNT_BITS):
            if (os.system(ENC1_CMD_PARTIAL.format(context_size)) != 0 or
                os.system(BWT_CMD) != 0 or
                os.system(ENC2_CMD_PARTIAL.format(count_bits)) != 0 or
                os.system(DEC2_CMD_PARTIAL.format(count_bits)) != 0 or
                os.system(UNBWT_CMD) != 0 or
                os.system(DEC1_CMD_PARTIAL.format(context_size)) != 0 or
                os.system(CHECK_CMD) != 0):
                print("Error for [{}, {}]".format(context_size, count_bits))
            else:
                ratio = os.path.getsize(ENC2_FILE) / RAW_SIZE
                results["{} {}".format(context_size, count_bits)] = ratio
                print("[{}, {}]: {}".format(context_size, count_bits, ratio))
    with open("bitwise_ppm_flat_bwt_rle.json", "w", encoding="utf-8") as file:
        file.write(json.dumps(results))

if __name__ == "__main__":
    main()
