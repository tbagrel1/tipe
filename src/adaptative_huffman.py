#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Adaptative huffman compression algorithm."""

NO_CHILD = None
NO_PARENT = None
LEFT = True
RIGHT = False
NO_SIDE = None
NO_VALUE = None
SPECIAL_VALUE = True


class BiNode(object):
    """Represents a node of a bidirectional binary tree."""

    def __init__(
        self,
        parent=NO_PARENT,
        side=NO_SIDE,
        left_child=NO_CHILD,
        right_child=NO_CHILD,
        weight=0,
        value=NO_VALUE,
        position=0):
        """Init method."""
        super().__init__()
        self._parent = parent
        self._side = side
        self._left_child = left_child
        self._right_child = right_child
        self._weight = weight
        self._value = value
        self._position = position

    def has_left_child(self):
        """Returns True iif this node has a left child."""
        return self._left_child != NO_CHILD

    def has_right_child(self):
        """Returns False iif this node has a right child."""
        return self._right_child != NO_CHILD

    def get_parent_side(self):
        """Returns parent and side of the current node."""
        return self._parent, self._side

    def set_parent_side(self, new_parent, new_side):
        """Sets the parent and side of the current node."""
        self._parent = new_parent
        self._side = new_side
        if self._parent != NO_PARENT:
            if self._side == LEFT:
                self._parent._left_child = self
            elif self._side == RIGHT:
                self._parent._right_child = self
            self.update_position(self._parent._position)
        else:
            self.update_position()

    def update_position(self, new_pos=0):
        """Update position of the current node and its children."""
        self._position = new_pos
        if self.has_left_child():
            self._left_child.update_position(2 * self._position + 1)
        if self.has_right_child():
            self._right_child.update_position(2 * self._position + 2)

    def get_code(self):
        """Returns the code of the current node."""
        depth = 0
        code = 0
        node = self
        while node._parent != NO_PARENT:  # While node is not the root
            if self._side == RIGHT:
                code += 2 ** depth
            depth += 1
            node = node._parent
        return (depth, code)


class AdaptativeHuffmanTree(object):
    """Represents an adaptative Huffman structure to compress text."""

    def __init__(self):
        """Init method."""
        super().__init__()
        self._root = BiNode(
            parent=NO_PARENT,
            side=NO_SIDE,
            left_child=NO_CHILD,
            right_child=NO_CHILD,
            weight=0,
            value=SPECIAL_VALUE,
            position=0)
        self._weight_index = {
            self._root._weight: [self._root]
        }
        self._value_index = {
            self._root._value: self._root
        }

    def inc_weight(self, node):
        """Increases the weight of the specified node."""
        self._weight_index[node._weight].remove(node)
        ## CHANGE FOR PERFORMANCE
        # if not self._weight_index[node._weight]:
        #     self._weight_index.pop(node._weight)
        ## END CHANGE FOR PERFORMANCE
        node._weight += 1
        if node._weight in self._weight_index:
            self._weight_index[node._weight].append(node)
        else:
            self._weight_index[node._weight] = [node]

    def inc_node(self, node):
        """Increases the specified node in the tree."""
        if node != NO_PARENT:
            top_node = self.get_top_node_with_weight(node._weight)
            if (top_node != node and
                top_node != node._parent):
                self.swap_nodes(node, top_node)
            self.inc_weight(node)
            self.inc_node(node._parent)

    def new_node(self, new_value):
        """Creates a new node in the tree holding the specified value."""
        special_leaf = self._value_index[SPECIAL_VALUE]
        new_value_leaf = BiNode(
            parent=special_leaf,
            side=LEFT,
            left_child=NO_CHILD,
            right_child=NO_CHILD,
            weight=0,
            value=new_value,
            position=2 * special_leaf._position + 1)
        new_special_leaf = BiNode(
            parent=special_leaf,
            side=RIGHT,
            left_child=NO_CHILD,
            right_child=NO_CHILD,
            weight=0,
            value=SPECIAL_VALUE,
            position=2 * special_leaf._position + 2)
        special_leaf._left_child = new_value_leaf
        special_leaf._right_child = new_special_leaf
        special_leaf._value = NO_VALUE
        self._weight_index[0].append(new_value_leaf)
        self._weight_index[0].append(new_special_leaf)
        self.inc_node(new_value_leaf)

    def code_char(self, char):
        """Returns a tuple (length, bit_sequence) where bit_sequence is an
        int corresponding to the bit sequence of the coded char and length
        the length of this sequence."""
        if char in self._value_index:
            result = self._value_index[char].get_code()
            self.inc_node(self._value_index[char])
            return result
        else:
            result = (7, ord(char))
            self.new_node(char)
            return result

    def code_text(self, text):
        """Returns a ready-to-write list corresponding to the compressed
        input text."""
        return compact_codes([self.code_char(c) for c in text])

    def get_top_node_with_weight(self, weight):
        """Returns the highest node in the tree with the specified weight."""
        nodes_with_weight = self._weight_index[weight]
        return min(nodes_with_weight, key=lambda x: x._position)

    def swap_nodes(self, node_a, node_b):
        """Swaps the specified nodes in the tree."""
        node_a_information = node_a.get_parent_side()
        node_b_information = node_b.get_parent_side()
        node_a.set_parent_side(*node_b_information)
        node_b.set_parent_side(*node_a_information)


def compact_codes(codes):
    """Compacts the list of codes and returns a ready-to-write list."""
    result_list = []
    length = 0
    value = 0
    for (l, c) in codes:
        length += l
        value = (value << l) + c
        if length >= 8:
            length -= 8
            result_list.append(value >> length)
            value = value - (value >> length << length)
    result_list.append(value << (8 - length))
    result_list = [length] + result_list
    return result_list

def main():
    """Main function."""
    pass


if __name__ == "__main__":
    main()
