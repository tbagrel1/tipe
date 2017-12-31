## Input format

The input format is a simple *Python* string (`str`)

## Output format

The output format is a `list` of 8-bit `int` where the first one represent the number of meaningful bits in the last `int`

For example, if the message is 27 bits long, the corresponding `list` will be

```text
00000011 xxxxxxxx xxxxxxxx xxxxxxxx xxx00000
```

