#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Bench BWT + RLE."""

import os
import numpy as np
import bitwise_ppm_bwt.bench_plot as myplt
import json

MAX_COUNT_BITS = 7
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
    ratios = [[], []]
    effs = [[], []]
    factors = [[], []]
    results = {}
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
            ratios[0].append(count_bits)
            ratios[1].append(ratio)
            effs[0].append(count_bits)
            effs[1].append(1 - ratio)
            factors[0].append(count_bits)
            factors[1].append(1 / ratio)
            results[str(count_bits)] = ratio
    with open("bwt_rle.json", "w", encoding="utf-8") as file:
        file.write(json.dumps(results))
    myplt.plot(
        data_sets=[ratios, effs, factors],
        y_min=0,
        y_max=4,
        y_ticks=np.arange(0, 4 + 0.25, 0.25),
        lims=[ZIP_RATIO, 1 - ZIP_RATIO, 1 / ZIP_RATIO],
        lim_lines=["--", "--", "--"],
        title="bwt_rle",
        x_label="count_bits",
        y_label="",
        colors=myplt.DEFAULT_COLORS,
        markers=["+", "+", "+"],
        lines=["-", "-", "-"],
        labels=["comp_ratio", "comp_efficiency", "comp_factor"],
        save_path="bwt_rle.png",
        show=True
    )

if __name__ == "__main__":
    main()
