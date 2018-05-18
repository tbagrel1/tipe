#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Bench BWT + RLE."""

import os
import matplotlib.pyplot as plt

MAX_COUNT_BITS = 8
EXE = "./rle_exe"
RAW_FILE = "calgary_rle.tar"
BWT_FILE = "calgary_rle.bwt"
ENC_FILE = "calgary_rle.enc"
DEC_FILE = "calgary_rle.dec"
UNBWT_FILE = "calgary_rle.unbwt"
ZIP_FILE = "calgary_rle.zip"
JUNK_FILE = "junk.log"
RAW_SIZE = os.path.getsize(RAW_FILE)
ZIP_RATIO = os.path.getsize(ZIP_FILE) / RAW_SIZE
BWT_CMD = "./bwt {} {}".format(RAW_FILE, BWT_FILE)
ENC_CMD_PARTIAL = "{} enc {} {} {{}} > {}".format(
    EXE, BWT_FILE, ENC_FILE, JUNK_FILE)
DEC_CMD_PARTIAL = "{} dec {} {} {{}} > {}".format(
    EXE, ENC_FILE, DEC_FILE, JUNK_FILE)
UNBWT_CMD = "./unbwt {} {}".format(DEC_FILE, UNBWT_FILE)
CHECK_CMD = "cmp -s {} {}".format(RAW_FILE, UNBWT_FILE)

def main():
    """Launcher."""
    plt.figure()
    for count_bits in range(1, MAX_COUNT_BITS + 1):
        if (os.system(BWT_CMD) != 0 or
            os.system(ENC_CMD_PARTIAL.format(count_bits)) != 0 or
            os.system(DEC_CMD_PARTIAL.format(count_bits)) != 0 or
            os.system(UNBWT_CMD) != 0 or
            os.system(CHECK_CMD) != 0):
            print("Error for <{}>".format(count_bits))
        else:
            ratio = os.path.getsize(ENC_FILE) / RAW_SIZE
            print("[{}]: {}".format(count_bits, ratio))
            plt.plot(count_bits, ratio, "+--r")
            plt.plot(count_bits, 1 - ratio, "+--b")
            plt.plot(count_bits, 1 / ratio, "+--g")
    plt.plot([0, MAX_COUNT_BITS], [ZIP_RATIO, ZIP_RATIO], "--r")
    plt.plot([0, MAX_COUNT_BITS], [1 - ZIP_RATIO, 1 - ZIP_RATIO], "--b")
    plt.plot([0, MAX_COUNT_BITS], [1 / ZIP_RATIO, 1 / ZIP_RATIO], "--g")
    plt.axis([0, MAX_COUNT_BITS, 0, 4])
    plt.savefig("bwt_rle.png")
    plt.show()

if __name__ == "__main__":
    main()
