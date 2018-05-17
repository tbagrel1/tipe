package org.tbagrel.treeutil.binode;

import org.tbagrel.treeutil.binode.exceptions.InvalidPositionBiNodeException;

/**
 * Class used to represent a binary tree which can be ran forwards or backwards
 * with a position and weight indexation.
 * @param <T>
 *     Type of the value that node will hold
 */
public class BiNode<T> {
    /**
     * Static position value meaning this node is the root.
     */
    public static final int ROOT_POSITION = 0;

    /**
     * Static BiNode value meaning that this node has no parent.
     */
    public static final BiNode NO_PARENT = null;

    /**
     * Static BiNode value meaning that this node has no child
     */
    public static final BiNode NO_CHILD = null;

    /**
     * Static childOfSide value meaning that this node is the left child of its
     * parent.
     */
    public static final boolean LEFT_CHILD = true;

    /**
     * Static childOfSide value meaning that this node is the right child of
     * its parent.
     */
    public static final boolean RIGHT_CHILD = false;

    /**
     * Static childOfSide value for node without parent.
     */
    public static final boolean NO_SIDE = false;

    /**
     * Value held by the node.
     */
    protected T _value;

    /**
     * Position of the node in the hierarchy: - root: 0; - left child: 2 *
     * parent position + 1; - right child: 2 * parent position + 2.
     */
    protected Integer _position;

    /**
     * Weight of the node.
     */
    protected Integer _weight;

    /**
     * Left child of the node.
     */
    protected BiNode<T> _childLeft;

    /**
     * Right child of the node.
     */
    protected BiNode<T> _childRight;

    /**
     * Parent of the node.
     */
    protected BiNode<T> _parent;

    /**
     * Boolean saying if the node is the left child or right child of its
     * parent.
     */
    protected boolean _childOfSide;

    /**
     * Default constructor. The created node has no child and no parent.
     * @param value
     *     Value held by the node
     */
    public BiNode(T value) {
        this._value = value;
        this._position = 0;
        this._weight = 0;
        this._childLeft = BiNode.NO_CHILD;
        this._childRight = BiNode.NO_CHILD;
        this._parent = BiNode.NO_PARENT;
        this._childOfSide = BiNode.NO_SIDE;
    }

    /**
     * Constructor. The created node has no child.
     * @param value
     *     Value held by the node
     * @param parent
     *     Parent of the node
     * @param childOfSide
     *     Boolean saying if the node is the left child or right child of its
     *     parent
     */
    public BiNode(T value, BiNode<T> parent, boolean childOfSide)
        throws InvalidPositionBiNodeException {
        this._value = value;
        this._weight = 0;
        this._childLeft = BiNode.NO_CHILD;
        this._childRight = BiNode.NO_CHILD;
        this.setParentChildOfSide(parent, childOfSide);
    }

    /**
     * Constructor.
     * @param value
     *     Value held by the node
     * @param parent
     *     Parent of the node
     * @param childOfSide
     *     Boolean saying if the node is the left child or right child of its
     *     parent
     * @param childLeft
     *     Left child of the node
     * @param childRight
     *     Right child of the node
     */
    public BiNode(
        T value,
        BiNode<T> parent,
        boolean childOfSide,
        BiNode<T> childLeft,
        BiNode<T> childRight) throws InvalidPositionBiNodeException {
        this._value = value;
        this._weight = 0;
        this.setParentChildOfSide(parent, childOfSide);
        this.setChildLeft(childLeft);
        this.setChildRight(childRight);
    }

    /**
     * Position getter.
     * @return Position of the node
     */
    public Integer getPosition() {
        return this._position;
    }

    /**
     * Position setter.
     * @param newPosition
     *     Position of the node in the new schema
     * @throws InvalidPositionBiNodeException
     *     if the new position is strictly negative
     */
    public void setPosition(Integer newPosition)
        throws InvalidPositionBiNodeException {
        if (newPosition < 0) {
            System.out.println(this.toString() + "<<<" + newPosition);
            throw new InvalidPositionBiNodeException("Negative position.");
        }
        this._position = newPosition;
        if (this.hasChildLeft()) {
            this._childLeft.setPosition(2 * this._position + 1);
        }
        if (this.hasChildRight()) {
            this._childRight.setPosition(2 * this._position + 2);
        }
    }

    /**
     * Value getter.
     * @return Value held by the node
     */
    public T getValue() {
        return this._value;
    }

    /**
     * Value setter.
     * @param newValue
     *     New value held by the node
     */
    public void setValue(T newValue) {
        this._value = newValue;
    }

    /**
     * Weight getter.
     * @return Current weight of the node
     */
    public Integer getWeight() {
        return this._weight;
    }

    /**
     * Weight setter.
     * @param newWeight
     *     New weight of the node
     */
    public void setWeight(Integer newWeight) {
        this._weight = newWeight;
    }

    /**
     * Increment weight of the node by 1.
     */
    public void incrWeight() {
        ++this._weight;
    }

    /**
     * Child getter based on {@link BiNode#getChildLeft() getChildLeft} and
     * {@link BiNode#getChildRight() getChildRight} methods.
     * @param side
     *     Indicates child of which side will be returned
     * @return Left child of the node if side is true, right child of the node
     * otherwise
     */
    public BiNode<T> getChild(boolean side) {
        if (side == BiNode.LEFT_CHILD) {
            return this.getChildLeft();
        } else {
            return this.getChildRight();
        }
    }

    /**
     * Left child getter.
     * @return Left child of the node
     */
    public BiNode<T> getChildLeft() {
        return this._childLeft;
    }

    /**
     * Left child setter. Update position of each node in the left child
     * hierarchy.
     * @param newChildLeft
     *     New left child
     */
    public void setChildLeft(BiNode<T> newChildLeft)
        throws InvalidPositionBiNodeException {
        this._childLeft = newChildLeft;
        if (this.hasChildLeft()) {
            this._childLeft._parent = this;
            this._childLeft._childOfSide = BiNode.LEFT_CHILD;
            this._childLeft.setPosition(2 * this._position + 1);
        }
    }

    /**
     * Right child getter.
     * @return Right child of the node
     */
    public BiNode<T> getChildRight() {
        return this._childRight;
    }

    /**
     * Right child setter. Update position of each node in the right child
     * hierarchy.
     * @param newChildRight
     *     New right child
     */
    public void setChildRight(BiNode<T> newChildRight)
        throws InvalidPositionBiNodeException {
        this._childRight = newChildRight;
        if (this.hasChildRight()) {
            this._childRight._parent = this;
            this._childRight._childOfSide = BiNode.RIGHT_CHILD;
            this._childRight.setPosition(2 * this._position + 2);
        }
    }

    /**
     * Child setter based on {@link org.tbagrel.treeutil.binode
     * .BiNode#setChildLeft(BiNode)
     * setChildLeft} and {@link org.tbagrel.treeutil.binode
     * .BiNode#setChildRight(BiNode)
     * setChildRight} methods.
     * @param newChild
     *     New child of the node
     * @param side
     *     Indicates child of which side will be modified
     */
    public void setChild(BiNode<T> newChild, boolean side)
        throws InvalidPositionBiNodeException {
        if (side == BiNode.LEFT_CHILD) {
            this.setChildLeft(newChild);
        } else {
            this.setChildRight(newChild);
        }
    }

    /**
     * Parent getter.
     * @return Parent of the node
     */
    public BiNode<T> getParent() {
        return this._parent;
    }

    /**
     * Side getter.
     * @return Boolean indicates if the node is the left child or the right
     * child of its parent
     */
    public boolean getChildOfSide() {
        return this._childOfSide;
    }

    /**
     * Parent and side setter.
     * @param newParent
     *     New parent of the node
     * @param newChildOfSide
     *     Indicates if the node is the new left child or the new right child
     *     of its new parent
     */
    public void setParentChildOfSide(
        BiNode<T> newParent, boolean newChildOfSide)
        throws InvalidPositionBiNodeException {
        this._parent = newParent;
        this._childOfSide = newChildOfSide;
        if (this.hasParent()) {
            if (this._childOfSide == BiNode.LEFT_CHILD) {
                this._parent._childLeft = this;
                this.setPosition(2 * this._parent._position + 1);
            } else {
                this._parent._childRight = this;
                this.setPosition(2 * this._parent._position + 2);
            }
        }
    }

    /**
     * Tell if the specified node is the brother of this node.
     * @param testBrother
     *     Node to test with
     * @return true if both nodes are brothers, else otherwise
     */
    public boolean isBrother(BiNode<T> testBrother) {
        return (this._parent == testBrother._parent && this != testBrother);
    }

    /**
     * Tell if the node has a child in the side position.
     * @param side
     *     Side to check
     * @return true if the node has a child in the side position, else
     * otherwise
     */
    public boolean hasChild(boolean side) {
        if (side == BiNode.LEFT_CHILD) {
            return this.hasChildLeft();
        } else {
            return this.hasChildRight();
        }
    }

    /**
     * Tell if this node has a left child.
     * @return true if the node has a left child, else otherwise
     */
    public boolean hasChildLeft() {
        return this._childLeft != BiNode.NO_CHILD;
    }

    /**
     * Tell if this node has a right child.
     * @return true if the node has a right child, else otherwise
     */
    public boolean hasChildRight() {
        return this._childRight != BiNode.NO_CHILD;
    }

    /**
     * Return a string corresponding to the representation of the node.
     * @return Representation of the node
     */
    public String toString() {
        String
            parentString =
            this.hasParent() ? this._parent._position.toString() : "x";
        String
            childLeftString =
            this.hasChildLeft() ? this._childLeft._position.toString() : "x";
        String
            childRightString =
            this.hasChildRight() ? this._childRight._position.toString() : "x";
        String
            valueString =
            this._value != null ? this._value.toString() : "\0";
        return String.format(
            "^[%s]^\n[%s] >>>%s<<< (%s)\n<[%s]< >[%s]>",
            parentString,
            this._position.toString(),
            valueString,
            this._weight.toString(),
            childLeftString,
            childRightString);
    }

    /**
     * Tell if this node has a parent.
     * @return true if the node has a parent, else otherwise
     */
    public boolean hasParent() {
        return this._parent != BiNode.NO_PARENT;
    }
}
