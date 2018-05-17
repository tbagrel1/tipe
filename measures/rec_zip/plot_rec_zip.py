#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Plots measures about rec_zip experiment."""

import json
import matplotlib.pyplot as plt
import numpy as np

RESULT_FILE = "rec_zip_measures.txt"
PLOT_FILE = "rec_zip_measures.png"
DELTA_WINDOW = 2 ** 15

def main():
    """Main function."""
    with open(RESULT_FILE, "r", encoding="utf-8") as result:
        experiment = json.loads(result.read())
    init_file_name = experiment["file"]
    spaces = experiment["spaces"]
    plt.figure()
    plt.style.use('dark_background')
    plt.plot(range(len(spaces)), spaces, "+-", color=[64/256,208/256,224/256])
    plt.title(r"rec zip: $\tt{" + init_file_name + r"}$")
    plt.xlabel("passes")
    plt.xticks(np.arange(0, len(spaces), 1))
    plt.ylabel("space (bytes)")
    plt.axis([
        0, len(spaces) - 1,
        0, 4 * 10 ** 6])
    plt.savefig(PLOT_FILE, dpi=300, bbox_inches="tight")
    plt.show()

if __name__ == "__main__":
    main()

