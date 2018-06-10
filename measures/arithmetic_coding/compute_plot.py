#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Plots measures about arithmetic coding experiment."""

import bitwise_ppm_bwt.bench_plot as myplt
import numpy as np
import os

def experiment(p):
    """Gets result from the C experiment."""
    return float(os.popen("./compute {}".format(p)).read())

def main():
    """Main function."""
    P = np.arange(0.01, 0.99 + 0.01, 0.01)
    DATA = [experiment(p) for p in P]

    myplt.plot(
        x_min=0,
        x_max=1,
        x_ticks=np.arange(0, 1 + 0.1, 0.1),
        data_sets=[[P, DATA]],
        y_min=0,
        y_max=15,
        y_ticks=np.arange(0, 15 + 1, 1),
        lims=None,
        lim_lines=None,
        x_label="$p(X_i = 1)$",
        y_label="compression factor",
        colors=myplt.DEFAULT_COLORS,
        markers=[""],
        lines=["-"],
        labels=None,
        save_path="arithmetic_efficiency.png",
        show=True
    )

if __name__ == "__main__":
    main()

