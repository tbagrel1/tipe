package org.tbagrel.bitutil.bitarray;

import java.util.Iterator;
import java.util.NoSuchElementException;

/**
 * BitArray standard iterator.
 */
class BitArrayIterator implements Iterator<Boolean> {

    /**
     * Bitarray to be iterated over
     */
    protected final BitArray _bitarray;

    /**
     * Index of the next bit to be read.
     */
    protected int _index;

    /**
     * Signal if a remove operation can be called.
     */
    protected boolean _operationOk;

    /**
     * Default constructor. Takes the bitarray to be iterated over as
     * parameter.
     * @param bitarray
     *     Bitarray to be iterated over
     */
    BitArrayIterator(BitArray bitarray) {
        this._bitarray = bitarray;
        this._operationOk = false;
        this._index = 0;
    }

    /**
     * Check if the bitarray has more bits to be read.
     * @return true if the bitarray has more elements, false otherwise
     */
    public boolean hasNext() {
        return this._index != this._bitarray._length;
    }

    /**
     * Return the next bit in the bitarray.
     * @return Next bit
     * @throws java.util.NoSuchElementException
     *     if the bitarray has no more elements to be read
     */
    public Boolean next() throws NoSuchElementException {
        if (this._index >= this._bitarray._length) {
            throw new NoSuchElementException("Bitarray has no more elements " +
                                             "to be read.");
        }
        this._operationOk = true;
        return this._bitarray._bits[this._index++];
    }

    /**
     * Remove from the bitarray the last element read.
     * @throws java.lang.IllegalStateException
     *     if {@link org.tbagrel.bitutil.bitarray.BitArrayIterator#next() next}
     *     wasn't called yet, or if a remove operation was already called
     */
    public void remove() throws IllegalStateException {
        if (!this._operationOk) {
            throw new IllegalStateException("next() wasn't called yet, or a " +
                                            "remove operation was already " +
                                            "called.");
        }
        this._operationOk = false;
        this._bitarray.remove(this._index - 1);
    }
}
