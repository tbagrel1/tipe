#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Bench BWT + bitwise_ppm_flat."""

import os
import numpy as np
import bitwise_ppm_bwt.bench_plot as myplt
import json

MAX_CONTEXT_SIZE = 28
EXE = "./bitwise_ppm_flat_exe"
RAW_FILE = "calgary_flat.tar"
BWT_FILE = "calgary_flat.bwt"
ENC_FILE = "calgary_flat.enc"
DEC_FILE = "calgary_flat.dec"
UNBWT_FILE = "calgary_flat.unbwt"
ZIP_FILE = "calgary_flat.zip"
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
    for context_size in range(MAX_CONTEXT_SIZE + 1):
        if (os.system(BWT_CMD) != 0 or
            os.system(ENC_CMD_PARTIAL.format(context_size)) != 0 or
            os.system(DEC_CMD_PARTIAL.format(context_size)) != 0 or
            os.system(UNBWT_CMD) != 0 or
            os.system(CHECK_CMD) != 0):
            print("Error for <{}>".format(context_size))
        else:
            ratio = os.path.getsize(ENC_FILE) / RAW_SIZE
            print("[{}]: {}".format(context_size, ratio))
            ratios[0].append(context_size)
            ratios[1].append(ratio)
            effs[0].append(context_size)
            effs[1].append(1 - ratio)
            factors[0].append(context_size)
            factors[1].append(1 / ratio)
            results[str(context_size)] = ratio
    with open("bwt_bitwise_ppm_flat.json", "w", encoding="utf-8") as file:
        file.write(json.dumps(results))
    myplt.plot(
        data_sets=[ratios, effs, factors],
        y_min=0,
        y_max=4,
        y_ticks=np.arange(0, 4 + 0.25, 0.25),
        lims=[ZIP_RATIO, 1 - ZIP_RATIO, 1 / ZIP_RATIO],
        lim_lines=["--", "--", "--"],
        #title="bitwise_ppm_flat",
        x_label="context_size",
        y_label="",
        colors=myplt.DEFAULT_COLORS,
        markers=["+", "+", "+"],
        lines=["-", "-", "-"],
        labels=["comp_ratio", "comp_efficiency", "comp_factor"],
        save_path="bwt_bitwise_ppm_flat.png",
        show=True
    )

if __name__ == "__main__":
    main()
