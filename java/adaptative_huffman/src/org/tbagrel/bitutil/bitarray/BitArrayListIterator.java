package org.tbagrel.bitutil.bitarray;

import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * BitArray list iterator.
 */
class BitArrayListIterator extends BitArrayIterator
    implements ListIterator<Boolean> {

    /**
     * Boolean value which indicate that the last operation was a next() call
     */
    public static final boolean LAST_NEXT = true;

    /**
     * Boolean value which indicate that the last operation was a prev() call
     */
    public static final boolean LAST_PREV = false;

    /**
     * Store the last operation done type
     */
    protected boolean _prevOperationType;

    /**
     * Default constructor. Take the bitarray to be iterated over as
     * parameter.
     * @param bitarray
     *     Bitarray to be iterated over
     */
    BitArrayListIterator(BitArray bitarray) {
        super(bitarray);
        this._prevOperationType = false;
    }

    /**
     * Constructor. Take the bitarray to be iterated over and the starting
     * index of the reading as parameters.
     * @param bitarray
     *     Bitarray to be iterated over
     * @param startingIndex
     *     Index of the first bit which will be read by the {@link
     *     BitArrayListIterator#next() next} method
     * @throws java.lang.IndexOutOfBoundsException
     *     if the starting index is over the bitarray's bounds
     */
    BitArrayListIterator(BitArray bitarray, int startingIndex)
        throws IndexOutOfBoundsException {
        super(bitarray);
        this._prevOperationType = false;
        if (startingIndex < 0) {
            startingIndex = this._bitarray._length + startingIndex;
        }
        if (startingIndex < 0 || startingIndex > this._bitarray._length) {
            throw new IndexOutOfBoundsException("Invalid index.");
        }
        this._index = startingIndex;
    }

    /**
     * Return the next bit in the bitarray.
     * @return Next bit
     * @throws java.util.NoSuchElementException
     *     if the bitarray has no more elements to be read in this direction
     */
    public Boolean next() throws NoSuchElementException {
        if (this._index >= this._bitarray._length) {
            throw new NoSuchElementException("Bitarray has no more element " +
                                             "to be read in this direction.");
        }
        this._operationOk = true;
        this._prevOperationType = BitArrayListIterator.LAST_NEXT;
        return super.next();
    }

    /**
     * Remove the last read bit (by either {@link BitArrayListIterator#next()
     * next} or {@link BitArrayListIterator#previous() previous} methods).
     * @throws java.lang.IllegalStateException
     *     if if neither next nor previous have been called, or remove or add
     *     have been called after the last call to next or previous
     */
    public void remove() throws IllegalStateException {
        if (!this._operationOk) {
            throw new IllegalStateException("This method can't be called now" +
                                            ".");
        }
        this._operationOk = false;
        this._bitarray.remove(this.lastCallIndex());
    }

    /**
     * Return the index of the last read bit (by either {@link
     * BitArrayListIterator#next() next} or {@link
     * BitArrayListIterator#previous()
     * previous} methods).
     * @return Index of the last read bit
     */
    public int lastCallIndex() {
        int index;
        if (this._prevOperationType == BitArrayListIterator.LAST_NEXT) {
            index = this._index - 1;
        } else {
            index = this._index;
        }
        return index;
    }

    /**
     * Check if the bitarray still contains bit to iterate over backwards.
     * @return true if the bitarray can be iterated over backward, else
     * otherwise
     */
    public boolean hasPrevious() {
        return this._index > 0;
    }

    /**
     * Return the previous bit in the bitarray.
     * @return Previous bit
     * @throws java.util.NoSuchElementException
     *     if the bitarray has no more elements to be read in this direction
     */
    public Boolean previous() throws NoSuchElementException {
        if (this._index <= 0) {
            throw new NoSuchElementException("Bitarray has no more element " +
                                             "to be read in this direction.");
        }
        this._operationOk = true;
        this._prevOperationType = BitArrayListIterator.LAST_PREV;
        return this._bitarray._bits[--this._index];
    }

    /**
     * Return the index of the bit which will be returned by the next call to
     * the {@link BitArrayListIterator#next() next} method.
     * @return Index of the next bit returned by the next next() call
     */
    public int nextIndex() {
        return this._index;
    }

    /**
     * Return the index of the bit which will be returned by the next {@link
     * BitArrayListIterator#previous() previous} method call.
     * @return Index of the bit returned by the next previous() call
     */
    public int previousIndex() {
        return this._index - 1;
    }

    /**
     * Set the last read bit (by either {@link BitArrayListIterator#next()
     * next} or {@link BitArrayListIterator#previous() previous} methods) to
     * the specified value.
     * @param bit
     *     Value the last read bit had to be changed into
     * @throws java.lang.IllegalStateException
     *     if neither next nor previous have been called, or remove or add have
     *     been called after the last call to next or previous
     */
    public void set(Boolean bit) throws IllegalStateException {
        if (!this._operationOk) {
            throw new IllegalStateException("This method can't be called now" +
                                            ".");
        }
        this._bitarray.set(this.lastCallIndex(), bit);
    }

    /**
     * Insert the specified bit just after the last read bit (by either {@link
     * BitArrayListIterator#next() next} or {@link
     * BitArrayListIterator#previous()
     * previous} methods).
     * @param bit
     *     Bit to be inserted in the bitarray
     * @throws java.lang.IllegalStateException
     *     if neither next nor previous have been called, or remove or add have
     *     been called after the last call to next or previous
     */
    public void add(Boolean bit) throws IllegalStateException {
        if (!this._operationOk) {
            throw new IllegalStateException("This method can't be called now" +
                                            ".");
        }
        this._operationOk = false;
        this._bitarray.add(this.lastCallIndex() + 1, bit);
    }
}
