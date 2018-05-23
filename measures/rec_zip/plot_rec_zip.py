#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Plots measures about rec_zip experiment."""

import json
import bitwise_ppm_bwt.bench_plot as myplt
import numpy as np

RESULT_FILE = "rec_zip_measures.txt"
PLOT_FILE = "rec_zip_measures.png"
DELTA_WINDOW = 2 ** 15
MB = 1024 ** 2

def main():
    """Main function."""
    with open(RESULT_FILE, "r", encoding="utf-8") as result:
        experiment = json.loads(result.read())
    init_file_name = experiment["file"]
    spaces = experiment["spaces"]

    myplt.plot(
        data_sets=[[list(range(len(spaces))), [x / MB for x in
                                                     spaces]]],
        y_min=0,
        y_max=4,
        y_ticks=np.arange(0, 4 + 0.25, 0.25),
        lims=None,
        lim_lines=None,
        #title="rec_zip: calgary.tar",
        x_label="passes",
        y_label="size (MB)",
        colors=myplt.DEFAULT_COLORS,
        markers=["+"],
        lines=["-"],
        labels=None,
        save_path="rec_zip.png",
        show=True
    )

if __name__ == "__main__":
    main()

