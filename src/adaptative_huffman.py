#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Adaptative huffman compression algorithm."""


class Node(object):
    """Node for the adaptative Huffman structure."""

    def __init__(self, weight, index):
        """Init method."""
        super().__init__()
        self._weight = weight
        self._index = index


class AdaptativeHuffmanStructure(object):
    """Tree-like structure used to store the adaptative huffman tree."""

    def __init__(self, str_to_code):
        """Init method."""
        super().__init__()
        special_node = Node(0, 0)
        self._str_to_node = {"": special_node}
        self._node_set = [special_node]
        self._str_to_code = str_to_code
        self._code_to_str = {}
        for s in self._str_to_code:
            self._code_to_str[self._str_to_code[s]] = s

    def add_value(self, value):
        """Adds a node corresponding to the specified value in the structure.
        """
        node = Node(1, len(self._node_set))
        self._str_to_node[value] = node
        self._node_set.append(node)
        n = len(self._node_set)
        self._node_set[n - 1], self._node_set[n - 2] = \
            self._node_set[n - 2], self._node_set[n - 1]
        self._node_set[n - 1]._index = n - 1
        self._node_set[n - 2]._index = n - 2

    def incr_value(self, value):
        """Increments the node corresponding to the specified value in the
        structure."""
        node = self._str_to_node[value]
        i_init = node._index
        i = node._index
        while (i >= 1 and
               self._node_set[i - 1]._weight == self._node_set[i]._weight):
            i -= 1
        self._node_set[i_init], self._node_set[i] = \
            self._node_set[i], self._node_set[i_init]
        self._node_set[i_init]._index = i_init
        self._node_set[i]._index = i
        node._weight += 1

    def code(self, value):
        """Codes the specified value according to the adaptative huffman
        algorithm."""


def main():
    """Main function."""
    pass


if __name__ == "__main__":
    main()
