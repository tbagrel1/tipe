package org.tbagrel.treeutil.adaptativehuffmantree;

import org.tbagrel.bitutil.bitarray.BitArray;
import org.tbagrel.bitutil.bitarray.exceptions.NegativeSpaceBitArrayException;
import org.tbagrel.bitutil.bitarray.exceptions.NotEnoughSpaceBitArrayException;
import org.tbagrel.stringutil.basicstringutil.BasicStringUtil;
import org.tbagrel.stringutil.basicstringutil.exceptions
    .InvalidLengthBSUException;
import org.tbagrel.treeutil.adaptativehuffmantree.exceptions.*;
import org.tbagrel.treeutil.binode.BiNode;
import org.tbagrel.treeutil.binode.exceptions.InvalidPositionBiNodeException;

import java.util.*;

/**
 * Class used to represent and build a huffman tree using the dynamic update.
 * @param <T>
 *     Type of values to be stored in leaves.
 */
public class AdaptativeHuffmanTree<T> {

    /**
     * Constant meaning that one node has no value.
     */
    public final T _NO_VALUE;

    /**
     * Constant meaning that one node is the special leaf.
     */
    public final T _SPECIAL_VALUE;

    /**
     * Map containing list of nodes sharing the same weight.
     */
    public final Map<Integer, List<BiNode<T>>> _weightMap;

    /**
     * Map containing default bit sequence for new values.
     */
    protected final Map<T, BitArray> _defaultCodeMap;

    /**
     * Map containing default bit sequence for new values.
     */
    protected final Map<BitArray, T> _defaultReversedCodeMap;

    /**
     * Store the length of the default codes for values.
     */
    protected final int _defaultCodeLength;

    /**
     * Root of the binary tree.
     */
    protected final BiNode<T> _root;

    /**
     * Map containing leaves of the binary tree indexed by value.
     */
    protected final Map<T, BiNode<T>> _valueMap;

    /**
     * Special leaf of the binary tree.
     */
    protected BiNode<T> _specialLeaf;

    /**
     * Default constructor.
     * @param noValue
     *     T value meaning that a node is not a leaf.
     * @param specialValue
     *     T value meaning that a node is the special leaf.
     * @param defaultCodeMap
     *     Map containing default bit sequence for new values.
     */
    public AdaptativeHuffmanTree(
        T noValue, T specialValue, Map<T, BitArray> defaultCodeMap)
        throws InvalidCodeMapAHTException {
        this._NO_VALUE = noValue;
        this._SPECIAL_VALUE = specialValue;
        this._defaultCodeMap = defaultCodeMap;
        this._defaultCodeLength =
            ((BitArray) this._defaultCodeMap.values()
                                            .toArray()[0]).getLength();
        this._defaultReversedCodeMap = new HashMap<>();
        for (Map.Entry<T, BitArray> e : this._defaultCodeMap.entrySet()) {
            if (e.getValue().getLength() != this._defaultCodeLength) {
                throw new InvalidCodeMapAHTException("All bitarrays in the " +
                                                     "code map don't have " +
                                                     "the same length.");
            }
            this._defaultReversedCodeMap.put(e.getValue(), e.getKey());
        }

        this._specialLeaf = new BiNode<>(this._SPECIAL_VALUE);
        this._root = this._specialLeaf;
        this._valueMap = new HashMap<>();
        this._valueMap.put(this._SPECIAL_VALUE, this._specialLeaf);
        this._weightMap = new HashMap<>();
        List<BiNode<T>> listZero = new LinkedList<>();
        listZero.add(this._root);
        this._weightMap.put(this._root.getWeight(), listZero);
    }

    /**
     * Tell if the specified node is a leaf or not.
     * @param node
     *     Node to be tested
     * @return true if the specified node is a leaf, else otherwise
     */
    public boolean isLeaf(BiNode<T> node) {
        return node.getValue() != this._NO_VALUE;
    }

    /**
     * Process the specified value (add value in the tree if not present, or
     * increment the corresponding node).
     * @param value
     *     Value to be processed
     * @return Bitarray corresponding to the specified value at this state
     * @throws ValueNotFoundAHTException
     *     if the value is not present in the AHT yet and cannot be found in
     *     the default code map
     */
    public BitArray encodeValue(T value)
        throws NodeNotFoundAHTException, InvalidPositionBiNodeException,
               ValueNotFoundAHTException, NegativeSpaceBitArrayException,
               NotEnoughSpaceBitArrayException {
        BiNode<T> node, pathNode;
        BitArray result = new BitArray();
        if (this._valueMap.containsKey(value)) {
            pathNode = this._valueMap.get(value);
            while (pathNode.hasParent()) {
                result.push(pathNode.getChildOfSide());
                pathNode = pathNode.getParent();
            }
            node = this._valueMap.get(value);
            result = result.reversed();
        } else {
            pathNode = this._specialLeaf;
            while (pathNode.hasParent()) {
                result.push(pathNode.getChildOfSide());
                pathNode = pathNode.getParent();
            }
            result = result.reversed();
            BitArray defaultCode = this._defaultCodeMap.get(value);
            if (defaultCode == null) {
                throw new ValueNotFoundAHTException("The specified value was" +
                                                    " not found in the " +
                                                    "default code map.");
            }
            result.append(defaultCode);
            this.addNewValueNode(value);
            node = this._specialLeaf.getParent().getParent();
        }
        if (node != BiNode.NO_PARENT) {
            this.safeIncrement(node);
        }
        return result;
    }

    /**
     * Encode the specified list of values using the adaptative Huffman
     * algorithm.
     * @param valueList
     *     List of values to be processed
     * @return Bitarray corresponding to the data
     */
    public BitArray encode(List<T> valueList)
        throws NodeNotFoundAHTException, InvalidPositionBiNodeException,
               ValueNotFoundAHTException, NegativeSpaceBitArrayException,
               NotEnoughSpaceBitArrayException {
        List<BitArray> bitarrayList = new LinkedList<>();
        BitArray tempBitarray;
        int totalLength = 0;
        for (T value : valueList) {
            tempBitarray = this.encodeValue(value);
            totalLength += tempBitarray.getLength();
            bitarrayList.add(tempBitarray);
        }
        BitArray result = new BitArray(totalLength);
        for (BitArray bitarray : bitarrayList) {
            result.append(bitarray);
        }
        return result;
    }

    /**
     * Decode the specified bitarray using the adaptative Huffman algorithm.
     * @param bitarray
     *     Bitarray to be processed
     * @return List of values corresponding to the encoded data
     */
    public List<T> decode(BitArray bitarray)
        throws ValueNotFoundAHTException, InvalidPositionBiNodeException,
               NodeNotFoundAHTException {
        List<T> result = new ArrayList<>();
        BiNode<T> node;
        while (!bitarray.isReadingFinished()) {
            node = this._root;
            while (node.getValue() == this._NO_VALUE) {
                node = node.getChild(bitarray.read());
            }
            if (node == this._specialLeaf) {
                BitArray defaultCode = new BitArray();
                for (int i = 0; i < this._defaultCodeLength; ++i) {
                    defaultCode.push(bitarray.read());
                }
                T
                    associatedValue =
                    this._defaultReversedCodeMap.get(defaultCode);
                if (associatedValue == null) {
                    throw new ValueNotFoundAHTException("This code doesn't " +
                                                        "match any values in" +
                                                        " the reversed map.");
                }
                result.add(associatedValue);
                this.addNewValueNode(associatedValue);
                node = this._specialLeaf.getParent().getParent();
            } else {
                result.add(node.getValue());
            }
            this.safeIncrement(node);
        }
        return result;
    }

    /**
     * Swap the two specified nodes by swapping their parents and respective
     * positions.
     * @param nodeA
     *     First node
     * @param nodeB
     *     Second node
     */
    public void swapNodes(BiNode<T> nodeA, BiNode<T> nodeB)
        throws InvalidPositionBiNodeException {
        BiNode<T> parentA = nodeA.getParent();
        boolean sideA = nodeA.getChildOfSide();
        nodeA.setParentChildOfSide(nodeB.getParent(), nodeB.getChildOfSide());
        nodeB.setParentChildOfSide(parentA, sideA);
    }

    /**
     * Increment the node path starting with the specified node respecting the
     * AHT hierarchy.
     * @param node
     *     First node to be incremented
     */
    public void safeIncrement(BiNode<T> node)
        throws NodeNotFoundAHTException, InvalidPositionBiNodeException {
        if (node != BiNode.NO_PARENT) {
            BiNode<T> topNode = this.firstNodeWithWeight(node.getWeight());
            if (topNode != node &&
                !(node.hasParent() && topNode == node.getParent())) {
                this.swapNodes(node, topNode);
            }
            this.updateNodeWeight(node, node.getWeight() + 1);
            this.safeIncrement(node.getParent());
        }
    }

    /**
     * Find the node with the lowest position value with the specified weight.
     * @param weight
     *     Weight the node must have
     * @return Node with the specified weight and with the lowest position
     * value
     * @throws NodeNotFoundAHTException
     *     if no node was found with the specified weight
     */
    public BiNode<T> firstNodeWithWeight(Integer weight)
        throws NodeNotFoundAHTException {
        if (!this._weightMap.containsKey(weight)) {
            throw new NodeNotFoundAHTException("No node can be found with " +
                                               "the specified weight.");
        }
        List<BiNode<T>> nodeList = this._weightMap.get(weight);
        if (nodeList.isEmpty()) {
            throw new NodeNotFoundAHTException("No node can be found with " +
                                               "the specified weight.");
        }
        BiNode<T> minNode = nodeList.get(0);
        Integer minPosition = minNode.getPosition();
        for (BiNode<T> node : nodeList) {
            Integer pos = node.getPosition();
            if (pos < minPosition) {
                minNode = node;
                minPosition = pos;
            }
        }
        return minNode;
    }

    /**
     * Change value of the specified node and reverberate changes in the AHT
     * maps.
     * @param node
     *     Node whose value must be changed
     * @param newValue
     *     New value for the node
     */
    public void updateNodeValue(BiNode<T> node, T newValue) {
        if (this.isLeaf(node)) {
            this._valueMap.remove(node.getValue());
        }
        node.setValue(newValue);
        if (this.isLeaf(node)) {
            this._valueMap.put(newValue, node);
        }
    }

    /**
     * Change weight of the specified node and reverberate changes in the AHT
     * maps.
     * @param node
     *     Node whose weight must be changed
     * @param newWeight
     *     New weight for the node
     */
    public void updateNodeWeight(BiNode<T> node, Integer newWeight) {
        List<BiNode<T>> listNode = this._weightMap.get(node.getWeight());
        if (listNode != null) {
            listNode.remove(node);
            if (listNode.isEmpty()) {
                this._weightMap.remove(node.getWeight());
            }
        }
        node.setWeight(newWeight);
        listNode = this._weightMap.get(node.getWeight());
        if (listNode != null) {
            listNode.add(node);
        } else {
            listNode = new LinkedList<>();
            listNode.add(node);
            this._weightMap.put(node.getWeight(), listNode);
        }
    }

    /**
     * Return a new node with specified value and weight and add it in the AHT
     * maps.
     * @param value
     *     Value for the new node
     * @param weight
     *     Weight for the new node
     * @return New node with specified parameters
     */
    public BiNode<T> newNode(T value, Integer weight) {
        BiNode<T> node = new BiNode<>(value);
        this.updateNodeValue(node, value);
        this.updateNodeWeight(node, weight);
        return node;
    }

    /**
     * Add a new leaf in the AHT with the specified value.
     * @param value
     *     Value for the new leaf
     */
    public void addNewValueNode(T value)
        throws InvalidPositionBiNodeException {
        BiNode<T> newValueNode = this.newNode(value, 1);
        BiNode<T> newSpecialNode = this.newNode(this._SPECIAL_VALUE, 0);
        this.updateNodeValue(this._specialLeaf, this._NO_VALUE);
        this.updateNodeWeight(this._specialLeaf, 1);
        this._specialLeaf.setChildLeft(newValueNode);
        this._specialLeaf.setChildRight(newSpecialNode);
        this._specialLeaf = newSpecialNode;
    }

    /**
     * Get the max depth of the subtree starting with the specified node.
     * @param node
     *     Node from which the depth is calculated
     * @return Depth of the subtree
     */
    public int getDepth(BiNode<T> node) {
        int
            depthLeft =
            node.hasChildLeft() ? this.getDepth(node.getChildLeft()) : 0;
        int
            depthRight =
            node.hasChildRight() ? this.getDepth(node.getChildRight()) : 0;
        return depthLeft > depthRight ? depthLeft : depthRight;
    }

    /**
     * Get the max depth of the AHT.
     * @return Depth of the AHT.
     */
    public int getDepth() {
        return getDepth(this._root);
    }

    /**
     * Return a representation of the AHT.
     * @return Representation of the AHT
     */
    public String toString() {
        String result;
        try {
            int
                centeringLength =
                BasicStringUtil.safeLength(this._root.getWeight());
            List<String> build = this.recStringBuilder("----",
                                                       "    " +
                                                       BasicStringUtil.mult
                                                           (" ",
                                                                            centeringLength),
                                                       this._root,
                                                       centeringLength,
                                                       this.getDepth(),
                                                       0);
            result = String.join("\n", build);
        } catch (InvalidLengthBSUException e) {
            result = "";
        }
        return result;
    }

    /**
     * Recursive building of the AHT representation
     * @param firstLinePrefix
     *     Prefix printed on the first line of the subtree
     * @param otherPrefix
     *     Prefix printed on all lines of the subtree excepted the first one
     * @param node
     *     Node corresponding to the root of the subtree
     * @param centeringLength
     *     Centering length for {@link BasicStringUtil#centered(Object, int)
     *     centered} method
     * @param totalDepth
     *     Total maximum depth of the binary tree
     * @param currentDepth
     *     Depth of the specified node in the binary tree
     * @return List of String containing the representation of the subtree
     * (each element in the list corresponds to one line)
     */
    protected List<String> recStringBuilder(
        String firstLinePrefix,
        String otherPrefix,
        BiNode<T> node,
        int centeringLength,
        int totalDepth,
        int currentDepth) throws InvalidLengthBSUException {
        List<String> result;
        if (this.isLeaf(node)) {
            result = new ArrayList<>();
            result.add(firstLinePrefix + String.format("%s <%s>",
                                                       BasicStringUtil
                                                           .centered(
                                                           node.getWeight(),
                                                           centeringLength),
                                                       node.getValue()
                                                           .toString()));
        } else {
            result = new ArrayList<>();
            int
                spaceFactor =
                (int) (Math.pow(2, totalDepth - currentDepth - 1));
            if (node.hasChildLeft()) {
                result.addAll(this.recStringBuilder("----",
                                                    "|   " +
                                                    BasicStringUtil.mult(" ",
                                                                         centeringLength),
                                                    node.getChildLeft(),
                                                    centeringLength,
                                                    totalDepth,
                                                    currentDepth + 1));
            } else {
                for (int i = 0; i < spaceFactor; ++i) {
                    result.add("");
                }
            }
            result.add("|   " + BasicStringUtil.mult(" ", centeringLength));
            if (node.hasChildRight()) {
                result.addAll(this.recStringBuilder("|---",
                                                    "    " +
                                                    BasicStringUtil.mult(" ",
                                                                         centeringLength),
                                                    node.getChildRight(),
                                                    centeringLength,
                                                    totalDepth,
                                                    currentDepth + 1));
            } else {
                for (int i = 0; i < spaceFactor; ++i) {
                    result.add("");
                }
            }
            result.set(0,
                       firstLinePrefix +
                       BasicStringUtil.centered(node.getWeight(),
                                                centeringLength) +
                       result.get(0));
            for (int i = 1; i < result.size(); ++i) {
                result.set(i, otherPrefix + result.get(i));
            }
        }
        return result;
    }
}
