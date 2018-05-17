#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Plots measures about rec_zip experiment."""

import json
import matplotlib.pyplot as plt

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
    plt.plot(range(len(spaces)), spaces, "+-r")
    plt.xlabel("passes on file \"{}\"".format(init_file_name))
    plt.ylabel("space (bytes)")
    plt.axis([
        0, len(spaces) - 1,
        min(spaces) - DELTA_WINDOW, max(spaces) + DELTA_WINDOW])
    plt.savefig(PLOT_FILE, dpi=300, bbox_inches="tight")

if __name__ == "__main__":
    main()

