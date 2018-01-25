# BWT File format specifications

## Output

+ Header: `INDEX_SIZE` bytes of the file representing `block_size` minus the size of the last block

+ Regular block : `INDEX_SIZE` bytes representing the decode key and `block_size` bytes representing the rearrangement of the chars

+ Last block : `INDEX_SIZE` bytes representing the decode key and up to `block_size` bytes representing the rearrangement of the chars.

+ Extension of a BWT file: `.bwt`
