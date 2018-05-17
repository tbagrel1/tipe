#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Optimizes escape_probs params for bitwise_ppm_exe."""

import os
import matplotlib.pyplot as plt

MAX_CONTEXT_SIZE = 28
EXE = "./bitwise_ppm_flat_exe"
RAW_FILE = "calgary_flat.tar"
ENC_FILE = "calgary_flat.enc"
DEC_FILE = "calgary_flat.dec"
ZIP_FILE = "calgary_flat.zip"
JUNK_FILE = "junk.log"
RAW_SIZE = os.path.getsize(RAW_FILE)
ZIP_RATIO = os.path.getsize(ZIP_FILE) / RAW_SIZE
ENC_CMD_PARTIAL = "{} enc {} {} {{}} > {}".format(
    EXE, RAW_FILE, ENC_FILE, JUNK_FILE)
DEC_CMD_PARTIAL = "{} dec {} {} {{}} > {}".format(
    EXE, ENC_FILE, DEC_FILE, JUNK_FILE)
CHECK_CMD = "cmp -s {} {}".format(RAW_FILE, DEC_FILE)

def main():
    """Launcher."""
    plt.figure()
    for context_size in range(MAX_CONTEXT_SIZE + 1):
        if (os.system(ENC_CMD_PARTIAL.format(context_size)) != 0 or
            os.system(DEC_CMD_PARTIAL.format(context_size)) != 0 or
            os.system(CHECK_CMD) != 0):
            print("Error for <{}>".format(context_size))
        else:
            ratio = os.path.getsize(ENC_FILE) / RAW_SIZE
            print("[{}]: {}".format(context_size, ratio))
            plt.plot(context_size, ratio, "or")
            plt.plot(context_size, 1 - ratio, "ob")
            plt.plot(context_size, 1 / ratio, "og")
    plt.plot([0, MAX_CONTEXT_SIZE], [ZIP_RATIO, ZIP_RATIO], "-r")
    plt.plot([0, MAX_CONTEXT_SIZE], [1 - ZIP_RATIO, 1 - ZIP_RATIO], "-b")
    plt.plot([0, MAX_CONTEXT_SIZE], [1 / ZIP_RATIO, 1 / ZIP_RATIO], "-g")
    plt.axis([0, MAX_CONTEXT_SIZE, 0, 4])
    plt.savefig("bitwise_ppm_flat.png")
    plt.show()

if __name__ == "__main__":
    main()
