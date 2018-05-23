#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Bench bitwise_ppm."""

import os
import bitwise_ppm_bwt.bench_plot as myplt
import numpy as np
import json

MAX_ORDER = 3
PROBS = [
    [0.0],
    [0.0, 3.8533032766681573e-05],
    [0.0, 0.010393130469946988, 0.002793776368376827],
    [0.0, 4.5097597279462256e-05, 0.06536406363888679, 0.050611927799512305]
]

ENC_CMD = \
    lambda x: (
        "./bitwise_ppm_exe_{} enc calgary_{}.tar calgary_{}.enc {}"
        .format(x, x, x, " ".join([str(p) for p in PROBS[x]])))
DEC_CMD = \
    lambda x: (
        "./bitwise_ppm_exe_{} dec calgary_{}.enc calgary_{}.dec {}"
        .format(x, x, x, " ".join([str(p) for p in PROBS[x]])))
CHECK_CMD = \
    lambda x: "cmp -s calgary_{}.tar calgary_{}.dec".format(x, x)
RAW_SIZE = os.path.getsize("calgary_0.tar")
ZIP_RATIO = os.path.getsize("calgary_0.zip") / RAW_SIZE

def main():
    """Launcher."""
    ratios = [[], []]
    effs = [[], []]
    factors = [[], []]
    results = {}
    for order in range(MAX_ORDER + 1):
        if (os.system(ENC_CMD(order)) != 0 or
            os.system(DEC_CMD(order)) != 0 or
            os.system(CHECK_CMD(order)) != 0):
            print("Error for <{}>".format(order))
        else:
            ratio = os.path.getsize("calgary_{}.enc".format(order)) / RAW_SIZE
            print("[{}]: {}".format(order, ratio))
            ratios[0].append(order)
            ratios[1].append(ratio)
            effs[0].append(order)
            effs[1].append(1 - ratio)
            factors[0].append(order)
            factors[1].append(1 / ratio)
            results[str(order)] = ratio
    with open("bitwise_ppm.json", "w", encoding="utf-8") as file:
        file.write(json.dumps(results))
    myplt.plot(
        data_sets=[ratios, effs, factors],
        y_min=0,
        y_max=4,
        y_ticks=np.arange(0, 4 + 0.25, 0.25),
        lims=[ZIP_RATIO, 1 - ZIP_RATIO, 1 / ZIP_RATIO],
        lim_lines=["--", "--", "--"],
        #title="bitwise_ppm",
        x_label="order",
        y_label="",
        colors=myplt.DEFAULT_COLORS,
        markers=["+", "+", "+"],
        lines=["-", "-", "-"],
        labels=["comp_ratio", "comp_efficiency", "comp_factor"],
        save_path="bitwise_ppm.png",
        show=True
    )

if __name__ == "__main__":
    main()
