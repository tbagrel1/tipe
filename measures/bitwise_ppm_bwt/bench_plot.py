#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Matplotlib interface to plot bench results."""

import matplotlib.pyplot as plt
import numpy as np


def plot(
        data_sets, y_min, y_max, y_ticks=None, lims=None,
        lim_lines=None, title="", x_label="", y_label="", colors=None,
        markers=None, lines=None, labels=None, data_kwargs={}, lim_kwargs={},
        save_path=None, show=True):
    """Plots the specified data."""
    if colors is None:
        colors = ["r"] * len(data_sets)
    if markers is None:
        markers = ["+"] * len(data_sets)
    if lines is None:
        lines = ["-"] * len(data_sets)
    if lims is not None:
        if lim_lines is None:
            lim_lines = ["--"] * len(lims)
    plt.figure()
    for_legend = []
    for data_set, color, marker, line, label in zip(
            data_sets, colors, markers, lines, labels):
        for_legend.append(
            plt.plot(
                *data_set, color=color, marker=marker,
                linestyle=line, label=label, **data_kwargs
            )[0])
    x_min = min([min(data_set[0]) for data_set in data_sets])
    x_max = max([max(data_set[0]) for data_set in data_sets])
    if lims:
        for lim, color, lim_line in zip(lims, colors, lim_lines):
            plt.plot([x_min, x_max], [lim, lim], color=color,
                     linestyle=lim_line, **lim_kwargs)
    plt.axis([x_min, x_max, y_min, y_max])
    plt.title(title)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.legend(handles=for_legend)
    if y_ticks is not None:
        plt.yticks(y_ticks)
    plt.xticks(np.arange(x_min, x_max + 1, 1))
    if save_path:
        plt.savefig(save_path, dpi=300, bbox_inches="tight")
    if show:
        plt.show()


def main():
    """Launcher."""
    pass


if __name__ == "__main__":
    main()
