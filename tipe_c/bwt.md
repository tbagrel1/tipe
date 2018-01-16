# BWT File format specifications

## Output

+ Header: `index_size` bytes of the file representing `block_size` minus the size of the last block

+ Regular block : `index_size` bytes representing the decode key and `block_size` bytes representing the rearrangement of the chars

+ Last block : `index_size` bytes representing the decode key and up to `block_size` bytes representing the rearrangement of the chars.

+ Extension of a BWT file: `.bwt`
