# Run-length encoding

## Main principle

The main principe of run-length encoding technique (RLE) is to replace a consecutive repetition of $n$ times the character $\mathsf{a}$ with the sequence $n\mathsf{a}$.

## Example

The sequence $\mathsf{aaabccabba}$ is replaced with $3\mathsf{a}1\mathsf{b}2\mathsf{c}1\mathsf{a}2\mathsf{b}1\mathsf{a}$

## Implementation details

In order to implement this technique, some choices have to be made. There, we chose to

+ code each number of occurences on 3 bits (up to $8$ times the character $\mathsf{a}$ with the composed sequence $n\mathsf{a}$)
+ code each character on 7 bits (standard ASCII)

Hence, each composed sequence will be coded on 10 bits.

## Goal

This technique may be worse than a naive ASCII coding. Thus, it should only be used to code data which holds sequences of repeted characters.

*Note:* the Burrows-Wheeler transform (BWT) tends to put the same characters next to each other so that the RLE performance can be improved.

## Input format

The input format is a simple *Python* string (`str`)

## Output format

The output format is a `list` of `int` where each int represents a `N_NB + 
N_CHAR` bits word.

## Sources

[Wikipedia](https://en.wikipedia.org/wiki/Run-length_encoding)
