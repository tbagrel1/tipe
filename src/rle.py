#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Run-length encoding algorithm."""

N_NB = 3
N_CHARS = 7


def encode(text):
    """Encodes the specified text (string) according to the RLE algorithm and
    returns a list of (N_NB + N_CHARS) bits int representing the original data.
    """
    lim = 2 ** N_NB
    (i, n) = (0, len(text))
    if n > 0:
        previous_char = text[0]
    encoded_data = []
    occ = 0
    while i < n:
        char = text[i]
        if char == previous_char and occ < lim:
            occ += 1
            i += 1
        else:
            encoded_data.append((occ - 1) * 2 ** N_CHARS + ord(previous_char))
            previous_char = char
            occ = 0
    if occ:
        encoded_data.append((occ - 1) * 2 ** N_CHARS + ord(previous_char))
    return encoded_data


def decode(encoded_data):
    """Decodes the specified encoded data according to the RLE algorithm."""
    text = ""
    for k in encoded_data:
        occ_m = k // 2 ** N_CHARS
        ichar = k - (occ_m * 2 ** N_CHARS)
        text += (occ_m + 1) * chr(ichar)
    return text


def main():
    """Main function."""
    original_text = "aaabccabba"
    print(original_text)
    encoded_data = encode(original_text)
    print(encoded_data)
    text = decode(encoded_data)
    print(text)


if __name__ == "__main__":
    main()
