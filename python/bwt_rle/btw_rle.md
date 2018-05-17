# Binding of BWT and RLE

## Main principle

The goal here is to bind BWT and RLE and to measure compression rates that can be reached with this process.

## Implementation details

The adjustment variable here is the size of BWT blocks (named `BLOCK_SIZE`)

## Input format

The input format is a simple *Python* string (`str`)

## Output format

The output format is a `list` of tuples `(decode_key, encoded_data)` where `decode_key` is the key required to rebuild the original text block (relative to BWT) and `encoded_data` is the result of RLE on the blended text block produced by BWT.

## Sources

[Wikipedia](https://en.wikipedia.org/wiki/Run-length_encoding)  
[Wikipedia](https://fr.wikipedia.org/wiki/Transform%C3%A9e_de_Burrows-Wheeler)

*Note:* We decided to work from the French *Wikipedia* version of the algorithm since the "French" version of the bijective BWT was simpler to implement.
