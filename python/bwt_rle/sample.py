#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Text sample."""

SAMPLE = ""

with open("sample.txt", "r", encoding="utf-8") as sample_file:
    SAMPLE = sample_file.read()
