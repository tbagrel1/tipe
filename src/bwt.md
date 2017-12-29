# Burrows-Wheeler transform

## Main principle

The Burrows-Wheeler transform is a bijective transform which can be applied on a data set in order to maximize the efficiency of a future compression on this data set.

With this transform, probability that same characters are placed next to each other is greatly improved. However, the BWT provides no size reduction at all, but just a rearrangement of the data.

## Implementation details

In order to effectively implements this transform, we first decided to work on 4096 chars blocks. However, with the naive implementation in *Python*, about 3 s is required to process the data block. So we finally decided to work on 512 chars blocks with this implementation. If the Burrows-Wheeler transform become useful for next operations, a new implementation will be needed.

## Input format

The input format is a simple *Python* string (`str`)

## Output format

The output format is a `list` of tuples `(decode_key, blended_block)` where 
`decode_key` is the key required to rebuild the original text block and 
`blended_block` the text block where same letters have a larger probability 
to be next to each other.

## Source

[Wikipedia](https://fr.wikipedia.org/wiki/Transform%C3%A9e_de_Burrows-Wheeler)

*Note:* We decided to work from the French *Wikipedia* version of the algorithm since the "French" version of the bijective BWT was simpler to implement.
