#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Matplotlib interface to plot bench results."""

import matplotlib.pyplot as plt
import numpy as np

COLOR = [197 / 256, 208 / 256, 208 / 256]
BG_COLOR = [16 / 256, 21 / 256, 21 / 256]
GRID_COLOR = [47 / 256, 52 / 256, 52 / 256]
DEFAULT_COLORS = [
    [64 / 256, 208 / 256, 224 / 256],
    COLOR,
    [95 / 256, 207 / 256, 180 / 256],
]
MARKER_SIZE = 3
LINEWIDTH = 1
FONT_SIZE = 10


def plot(
        data_sets, y_min, y_max, y_ticks=None, lims=None,
        lim_lines=None, title="", x_label="", y_label="", colors=None,
        markers=None, lines=None, labels=None, data_kwargs={}, lim_kwargs={},
        save_path=None, show=True):
    """Plots the specified data."""
    plt.rcParams.update({"font.size": FONT_SIZE})
    plt.rcParams.update({"font.family": "monospace", "font.monospace": [
        "Inconsolata"]})
    # plt.rcParams.update({"text.usetex": True})
    if colors is None:
        colors = ["r"] * len(data_sets)
    if markers is None:
        markers = ["+"] * len(data_sets)
    if labels is None:
        labels = [""] * len(data_sets)
        b_label = False
    else:
        b_label = True
    if lines is None:
        lines = ["-"] * len(data_sets)
    if lims is not None:
        if lim_lines is None:
            lim_lines = ["--"] * len(lims)
    fig = plt.figure(facecolor=BG_COLOR)
    for_legend = []
    for data_set, color, marker, line, label in zip(
            data_sets, colors, markers, lines, labels):
        for_legend.append(
            plt.plot(
                *data_set, color=color, marker=marker,
                linestyle=line, label=label, markersize=MARKER_SIZE,
                linewidth=LINEWIDTH,
                **data_kwargs
            )[0])
    x_min = min([min(data_set[0]) for data_set in data_sets])
    x_max = max([max(data_set[0]) for data_set in data_sets])
    if lims:
        for lim, color, lim_line in zip(lims, colors, lim_lines):
            plt.plot([x_min, x_max], [lim, lim], color=color,
                     linestyle=lim_line, markersize=MARKER_SIZE,
                     linewidth=LINEWIDTH, **lim_kwargs)
    plt.axis([x_min, x_max, y_min, y_max])
    plt.title(title)
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.grid(True, color=GRID_COLOR)
    if y_ticks is not None:
        plt.yticks(y_ticks)
    plt.xticks(np.arange(x_min, x_max + 1, 1))

    ax = plt.gca()
    ax.set_axis_bgcolor(BG_COLOR)
    ax.spines["left"].set_color(COLOR)
    ax.spines["right"].set_color(COLOR)
    ax.spines["bottom"].set_color(COLOR)
    ax.spines["top"].set_color(COLOR)
    ax.xaxis.label.set_color(COLOR)
    ax.tick_params(axis="x", colors=COLOR)
    ax.yaxis.label.set_color(COLOR)
    ax.tick_params(axis="y", colors=COLOR)

    if b_label:
        leg = plt.legend(handles=for_legend, loc=2)
        for text in leg.get_texts():
            text.set_color(COLOR)
        frame = leg.get_frame()
        frame.set_facecolor(BG_COLOR)
        frame.set_edgecolor(COLOR)

    if save_path:
        plt.savefig(save_path, dpi=300, bbox_inches="tight",
                    facecolor=fig.get_facecolor(), edgecolor="none")
    if show:
        plt.show()
    return plt

def main():
    """Launcher."""
    data = [
        0.9892210328733766,
        0.9789671266233766,
        0.9583591213474026,
        0.9315911627435065,
        0.9138494318181818,
        0.8895545860389611,
        0.8606635551948052,
        0.7776874492694805,
        0.7113864650974026,
        0.6780311485389611,
        0.6406579748376623,
        0.611711140422078,
        0.5801516842532467,
        0.5521433644480519,
        0.5242872362012987,
        0.5016855215097402,
        0.47462713068181817,
        0.4530679281655844,
        0.4333426339285714,
        0.4198546570616883,
        0.40796469155844156,
        0.39847047483766235,
        0.3901582792207792,
        0.38253221387987013,
        0.3749708299512987,
        0.37156934862012986,
        0.3691456980519481,
        0.37043171672077924,
        0.3715135450487013
    ]
    d1 = [[], []]
    d2 = [[], []]
    d3 = [[], []]
    for i, d in enumerate(data):
        d1[0].append(i)
        d1[1].append(d)
        d2[0].append(i)
        d2[1].append(1 - d)
        d3[0].append(i)
        d3[1].append(1 / d)
    plot(
        data_sets=[d1, d2, d3],
        y_min=0,
        y_max=4,
        y_ticks=np.arange(0, 4 + 0.25, 0.25),
        lims=[0.3, 1 - 0.3, 1 / 0.3],
        lim_lines=["--", "--", "--"],
        title="bitwise_ppm_flat",
        x_label="context_size",
        y_label="",
        colors=DEFAULT_COLORS,
        markers=["+", "+", "+"],
        lines=["-", "-", "-"],
        labels=["comp_ratio", "comp_efficiency", "comp_factor"],
        save_path="test.png",
        show=True
    )

if __name__ == "__main__":
    main()
