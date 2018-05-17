package org.tbagrel.bitutil.bitarray;

import org.jetbrains.annotations.NotNull;
import org.tbagrel.bitutil.bitarray.exceptions.*;

import java.util.*;
import java.util.function.UnaryOperator;

/**
 * Class to store a modular array of bits (represented by the Boolean class).
 */
public class BitArray implements List<Boolean> {
    /**
     * Default space allocated to the bitarray (in Boolean (bit) unit) when no
     * space is provided in the constructor.
     */
    public static final int DEFAULT_SPACE_INIT_FLAT = 8;

    /**
     * Space multiplicator used when length is already specified (eg: {@link
     * BitArray#BitArray(BitArray) copy constructor}).
     */
    public static final double DEFAULT_SPACE_INIT_PERCENT = 1.25;

    /**
     * Space multiplicator used when the bitarray space is no longer enough for
     * new pushed values.
     */
    public static final double DEFAULT_SPACE_INCR = 2.0;

    /**
     * Space/length ratio from which the bitarray internal array is shrunk.
     * @see BitArray#adjust()
     */
    public static final double DEFAULT_SHRINK_THRESHOLD = 2.5;

    /**
     * New space/length ratio used when the bitarray is shrunk
     */
    public static final double DEFAULT_SHRINK_PERCENT = 1.25;

    /**
     * Default bit value used when completing a bitarray with no additional
     * information
     */
    public static final Boolean DEFAULT_VALUE = Boolean.FALSE;

    /**
     * Length of the bitarray.
     */
    protected int _length;

    /**
     * Total space used by the bitarray (in Boolean (bit) unit).
     */
    protected int _space;

    /**
     * Next position of the bit returned by the {@link BitArray#read() read}
     * method.
     */
    protected int _readPosition;

    /**
     * Internal representation of the bitarray.
     */
    protected Boolean[] _bits;

    /**
     * Default constructor. Default space is set by the constant {@link
     * BitArray#DEFAULT_SPACE_INIT_FLAT DEFAULT_SPACE_INIT_FLAT}.
     */
    public BitArray() {
        this._length = 0;
        this._space = BitArray.DEFAULT_SPACE_INIT_FLAT;
        this._readPosition = 0;
        this._bits = new Boolean[this._space];
    }

    /**
     * Constructor with initial space specified
     * @param init_space
     *     Initial space used for the internal storage of the bitarray in
     *     Boolean (bit) unit
     * @throws org.tbagrel.bitutil.bitarray.exceptions
     * .NegativeSpaceBitArrayException
     *     if init_space is strictly negative
     */
    public BitArray(int init_space) throws NegativeSpaceBitArrayException {
        this._length = 0;
        if (init_space < 0) {
            throw new NegativeSpaceBitArrayException("Negative space for " +
                                                     "bitarray is not valid.");
        }
        this._space = init_space;
        this._readPosition = 0;
        this._bits = new Boolean[this._space];
    }

    /**
     * Copy constructor. Be careful, space of the original array is not kept.
     * @param bitarray
     *     Bitarray to be copied into the new bitarray instance
     */
    public BitArray(BitArray bitarray) {
        this._length = bitarray._length;
        this._space =
            (int) (bitarray._length * (BitArray.DEFAULT_SPACE_INIT_PERCENT));
        this._readPosition = 0;
        this._bits = new Boolean[this._space];
        System.arraycopy(bitarray._bits, 0, this._bits, 0, bitarray._length);
    }

    /**
     * Copy constructor, with new space specified.
     * @param bitarray
     *     Bitarray to be copied into the new bitarray
     * @param newSpace
     *     Space allowed for the new instance. It must be greater than the
     *     original bitarray length!
     * @throws NotEnoughSpaceBitArrayException
     *     if the specified space is not large enough to store the array to be
     *     copied
     * @throws NegativeSpaceBitArrayException
     *     if the specified space is negative
     */
    public BitArray(BitArray bitarray, int newSpace)
        throws NotEnoughSpaceBitArrayException,
               NegativeSpaceBitArrayException {
        this._length = bitarray._length;
        if (newSpace < 0) {
            throw new NegativeSpaceBitArrayException("Negative space for " +
                                                     "bitarray is not valid");
        }
        if (newSpace < bitarray._length) {
            throw new NotEnoughSpaceBitArrayException("Specified space is " +
                                                      "not enough to store " +
                                                      "the bitarray to be " +
                                                      "copied");
        }
        this._space = newSpace;
        this._readPosition = 0;
        this._bits = new Boolean[this._space];
        System.arraycopy(bitarray._bits, 0, this._bits, 0, bitarray._length);
    }

    /**
     * Create a new bitarray with a length of 8 (byte size) from the byte value
     * given as parameter.
     * @param value
     *     Decimal value to be converted to bitarray
     * @return Corresponding bitarray
     * @throws org.tbagrel.bitutil.bitarray.exceptions
     * .NegativeDecimalValueBitArrayException
     *     if the specified value is strictly negative
     */
    public static BitArray fromByte(byte value)
        throws NegativeDecimalValueBitArrayException,
               NegativeSpaceBitArrayException {
        BitArray bitarray = BitArray.fromBase10((long) value);
        for (int i = bitarray._length; i < 8; ++i) {
            bitarray.push(Boolean.FALSE);
        }
        return bitarray.reversed();
    }

    /**
     * Create a new bitarray with a length of 16 (short size) from the short
     * value given as parameter.
     * @param value
     *     Decimal value to be converted to bitarray
     * @return Corresponding bitarray
     * @throws org.tbagrel.bitutil.bitarray.exceptions
     * .NegativeDecimalValueBitArrayException
     *     if the specified value is strictly negative
     */
    public static BitArray fromShort(short value)
        throws NegativeDecimalValueBitArrayException,
               NegativeSpaceBitArrayException {
        BitArray bitarray = BitArray.fromBase10((long) value);
        for (int i = bitarray._length; i < 16; ++i) {
            bitarray.push(Boolean.FALSE);
        }
        return bitarray.reversed();
    }

    /**
     * Create a new bitarray with a length of 32 (int size) from the int value
     * given as parameter.
     * @param value
     *     Decimal value to be converted to bitarray
     * @return Corresponding bitarray
     * @throws org.tbagrel.bitutil.bitarray.exceptions
     * .NegativeDecimalValueBitArrayException
     *     if the specified value is strictly negative
     */
    public static BitArray fromInt(int value)
        throws NegativeDecimalValueBitArrayException,
               NegativeSpaceBitArrayException {
        BitArray bitarray = BitArray.fromBase10((long) value);
        for (int i = bitarray._length; i < 32; ++i) {
            bitarray.push(Boolean.FALSE);
        }
        return bitarray.reversed();
    }

    /**
     * Create a new bitarray with a length of 64 (long size) from the long
     * value given as parameter.
     * @param value
     *     Decimal value to be converted to bitarray
     * @return Corresponding bitarray
     * @throws org.tbagrel.bitutil.bitarray.exceptions
     * .NegativeSpaceBitArrayException
     *     if the specified value is strictly negative
     */
    public static BitArray fromLong(long value)
        throws NegativeDecimalValueBitArrayException,
               NegativeSpaceBitArrayException {
        BitArray bitarray = BitArray.fromBase10(value);
        for (int i = bitarray._length; i < 64; ++i) {
            bitarray.push(Boolean.FALSE);
        }
        return bitarray.reversed();
    }

    /**
     * Create a new bitarray with a length given by the size parameter and a
     * binary value corresponding to the long value given as parameter.
     * @param value
     *     Decimal value to be converted to bitarray
     * @param size
     *     Size of the bitarray
     * @return Corresponding bitarray
     * @throws NotEnoughSpaceBitArrayException
     *     if the specified size is not large enough to store the specified
     *     binary value
     * @throws NegativeSpaceBitArrayException
     *     if the specified size is negative
     * @throws org.tbagrel.bitutil.bitarray.exceptions
     * .NegativeDecimalValueBitArrayException
     *     if the specified value is strictly negative
     */
    public static BitArray fromCustom(long value, int size)
        throws NotEnoughSpaceBitArrayException, NegativeSpaceBitArrayException,
               NegativeDecimalValueBitArrayException {
        if (size < 0) {
            throw new NegativeSpaceBitArrayException("Negative size for " +
                                                     "bitarray is not valid.");
        }
        BitArray bitarray = BitArray.fromBase10(value);
        if (bitarray._length > size) {
            throw new NotEnoughSpaceBitArrayException("Specified size is " +
                                                      "lower than size " +
                                                      "required to store the" +
                                                      " " +
                                                      "specified value.");
        }
        for (int i = bitarray._length; i < size; ++i) {
            bitarray.push(Boolean.FALSE);
        }
        return bitarray.reversed();
    }

    /**
     * Create a new bitarray as little as possible to hold the decimal value
     * given as parameter. Be careful, the bitarray obtained is reversed!
     * @param value
     *     Decimal value to be converted to bitarray
     * @return Reversed corresponding bitarray
     * @throws org.tbagrel.bitutil.bitarray.exceptions
     * .NegativeDecimalValueBitArrayException
     *     if the specified value is strictly negative
     */
    protected static BitArray fromBase10(long value)
        throws NegativeDecimalValueBitArrayException {
        BitArray bitarray = new BitArray();
        Boolean bit;
        if (value < 0) {
            throw new NegativeDecimalValueBitArrayException("Negative " +
                                                            "decimal value " +
                                                            "is not allowed.");
        }
        if (value == 0) {
            bitarray.push(Boolean.FALSE);
        } else {
            while (value != 0) {
                bitarray.push((value % 2 != 0));
                value /= 2;
            }
        }
        return bitarray;
    }

    /**
     * Concatenate two bitarray into one.
     * @param bitarrayA
     *     First (left) bitarray to be concatenated
     * @param bitarrayB
     *     Second (right) bitarray to be concatenated
     * @return Concatenate bitarray of bitarrayA and bitarrayB
     */
    public static BitArray concat(BitArray bitarrayA, BitArray bitarrayB)
        throws NotEnoughSpaceBitArrayException,
               NegativeSpaceBitArrayException {
        BitArray result;
        result = new BitArray(bitarrayA,
                              (int) (BitArray.DEFAULT_SPACE_INIT_PERCENT *
                                     (bitarrayA._length + bitarrayB._length)));
        result.append(bitarrayB);
        return result;
    }

    /**
     * Length getter.
     * @return Length of the bitarray.
     */
    public int getLength() {
        return this._length;
    }

    /**
     * Space getter.
     * @return Space allowed for the internal storage of the bitarray
     */
    public int getSpace() {
        return this._space;
    }

    /**
     * Space setter.
     * @param newSpace
     *     New space allowed for the internal storage of the bitarray. Must be
     *     greater than the actual length of the bitarray!
     * @throws NotEnoughSpaceBitArrayException
     *     if the specified size is not large enough to store the current
     *     bitarray
     */
    public void setSpace(int newSpace) throws NotEnoughSpaceBitArrayException,
                                              NegativeSpaceBitArrayException {
        if (newSpace < 0) {
            throw new NegativeSpaceBitArrayException("Negative space for " +
                                                     "bitarray is not valid.");
        }
        if (newSpace < this._length) {
            throw new NotEnoughSpaceBitArrayException("The specified space " +
                                                      "is strictly lower " +
                                                      "than the bitarray " +
                                                      "size");
        }
        Boolean[] temp = this._bits;
        this._space = newSpace;
        this._bits = new Boolean[this._space];
        System.arraycopy(temp, 0, this._bits, 0, this._length);
    }

    /**
     * Bits getter.
     * @return Vanilla array internally used to store the bitarray.
     */
    public Boolean[] getBits() {
        return this._bits;
    }

    /**
     * ReadPosition getter.
     * @return Position of the next bit returned by the {@link BitArray#read()
     * read} method.
     */
    public int getReadPosition() {
        return this._readPosition;
    }

    /**
     * ReadPosition setter.
     * @param readPosition
     *     New position of the next bit returned by the {@link BitArray#read()
     *     read} method
     * @throws java.lang.IndexOutOfBoundsException
     *     if the specified read position is out of bounds
     */
    public void setReadPosition(int readPosition)
        throws IndexOutOfBoundsException {
        if (readPosition < 0) {
            readPosition = this._length + readPosition;
            if (readPosition < 0) {
                throw new IndexOutOfBoundsException("Invalid new read " +
                                                    "position.");
            }
            this._readPosition = readPosition;
        } else {
            if (readPosition > this._length) {
                throw new IndexOutOfBoundsException("Invalid new read " +
                                                    "position.");
            }
            this._readPosition = readPosition;
        }
    }

    /**
     * Compute the number or remaining reading operations before the end of the
     * reading.
     * @return Number of readings operations remaining before reaching the end
     * of the bitarray.
     * @see BitArray#read()
     */
    public int readingsRemaining() {
        return this._length - this._readPosition;
    }

    /**
     * Read a bit from the bitarray and increment the readPosition counter.
     * @return Bit read from the bitarray
     * @throws NoSuchElementException
     *     if reading of the bitarray is over
     */
    public Boolean read() throws NoSuchElementException {
        if (this.isReadingFinished()) {
            throw new NoSuchElementException("Reading is over; cannot " +
                                             "read another bit.");
        }
        return this._bits[this._readPosition++];
    }

    /**
     * Check if bitarray reading reached the end.
     * @return Boolean telling if readings operations reached the end of the
     * bitarray.
     */
    public boolean isReadingFinished() {
        return this._readPosition == this._length;
    }

    /**
     * Add a bit at the end of the bitarray.
     * @param bit
     *     Bit to be added at the end of the bitarray
     */
    public void push(Boolean bit) {
        this.add(this._length, bit);
    }

    /**
     * Add a bit at the beginning of the array and shift all following bits.
     * @param bit
     *     Bit to be added at the beginning of the bitarray
     */
    public void ins(Boolean bit) {
        this.add(0, bit);
    }

    /**
     * Remove the bit at the end of the bitarray and return it.
     * @return Popped bit
     */
    public Boolean pop() {
        return this.remove(this._length - 1);
    }

    /**
     * Check if too many space is used to internally store the bitarray. If
     * that's the case, shrink the space allowed by creating a smaller internal
     * array.
     * @see BitArray#DEFAULT_SHRINK_THRESHOLD
     * @see BitArray#DEFAULT_SHRINK_PERCENT
     */
    public void adjust() throws NotEnoughSpaceBitArrayException,
                                NegativeSpaceBitArrayException {
        if ((double) this._space / (double) this._length >
            BitArray.DEFAULT_SHRINK_THRESHOLD) {
            this.setSpace((int) (this._length *
                                 BitArray.DEFAULT_SHRINK_PERCENT));
        }
    }

    /**
     * Return a reversed version of the current bitarray (the end becomes the
     * beginning of the array etc).
     * @return Reversed bitarray
     */
    public BitArray reversed() throws NegativeSpaceBitArrayException {
        BitArray result;
        result = new BitArray(this._length);
        for (int i = this._length - 1; i >= 0; --i) {
            result.push(this._bits[i]);
        }
        return result;
    }

    /**
     * Increase space used to internally store the bitarray.
     * @see BitArray#DEFAULT_SPACE_INCR
     */
    public void addSpace() throws NotEnoughSpaceBitArrayException,
                                  NegativeSpaceBitArrayException {
        this.setSpace((int) (this._space * BitArray.DEFAULT_SPACE_INCR + 1));
    }

    /**
     * Set the bit at index i to the given value. i must be greater than length
     * of the bitarray. In this case, the bitarray is expanded and filled with
     * a default value until length is strictly greater than i.
     * @param i
     *     Index of the bit to be modified
     * @param bit
     *     New value for this bit
     * @see BitArray#DEFAULT_VALUE
     */
    public void safeSet(int i, Boolean bit)
        throws NotEnoughSpaceBitArrayException,
               NegativeSpaceBitArrayException {
        if (i < this._length) {
            this._bits[i] = bit;
        } else if (i < this._space) {
            for (int j = this._length; j < i; ++j) {
                this._bits[j] = BitArray.DEFAULT_VALUE;
            }
            this._bits[i] = bit;
            this._length = i + 1;
        } else {

            this.setSpace((int) (BitArray.DEFAULT_SPACE_INIT_PERCENT *
                                 (i + 1)));
            this.safeSet(i, bit);
        }
    }

    /**
     * Add the content of another bitarray to the right of the current
     * bitarray.
     * @param otherBitarray
     *     Bitarray to be added to the right
     */
    public void append(BitArray otherBitarray)
        throws NegativeSpaceBitArrayException,
               NotEnoughSpaceBitArrayException {
        if (this._length + otherBitarray._length <= this._space) {
            System.arraycopy(otherBitarray._bits,
                             0,
                             this._bits,
                             this._length,
                             otherBitarray._length);
            this._length += otherBitarray._length;
        } else {
            this.setSpace((int) (BitArray.DEFAULT_SPACE_INIT_PERCENT *
                                 (this._length + otherBitarray._length)));
            this.append(otherBitarray);
        }
    }

    /**
     * Return the hash code corresponding to the bitarray instance.
     * @return hash code (int value)
     */
    public int hashCode() {
        int result = 0;
        for (int i = 0; i < this._length; ++i) {
            result =
                (31 * result + (this._bits[i] ? 1 : 0)) % Integer.MAX_VALUE;
        }
        return result;
    }

    /**
     * Check if the specified object is equal to the current bitarray
     * instance.
     * @param o
     *     Object to be compared to current bitarray instance
     * @return true is the specified object is equal to the bitarray instance,
     * else otherwise
     */
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        } else if (this.getClass() != o.getClass()) {
            return false;
        } else {
            BitArray other = (BitArray) o;
            if (this._length != other._length) {
                return false;
            } else {
                for (int i = 0; i < this._length; ++i) {
                    if (!this._bits[i].equals(other._bits[i])) {
                        return false;
                    }
                }
                return true;
            }
        }
    }

    /**
     * Give a representation of the current bitarray.
     * @return String corresponding to the bitarray
     */
    public String toString() {
        StringBuilder c = new StringBuilder();
        c.append("[");
        c.append(this._length);
        c.append("(");
        c.append(this._space);
        c.append("):");
        for (int i = 0; i < this._length; ++i) {
            if (this._bits[i]) {
                c.append("1");
            } else {
                c.append("0");
            }
        }
        c.append("]");
        return c.toString();
    }

    /**
     * Same as {@link BitArray#getLength() getLength} method. Return the size
     * (length) of the bitarray.
     * @return Size (length) of the bitarray
     */
    public int size() {
        return this._length;
    }

    /**
     * Return true if the bitarray is empty, and else if it contains at least
     * one bit.
     * @return true if the bitarray contains not bit, else otherwise
     */
    public boolean isEmpty() {
        return this._length == 0;
    }

    /**
     * Return true if the bitarray contains the specified object.
     * @param o
     *     Object to be found in the bitarray
     * @return true if this object was found, else otherwise
     */
    public boolean contains(Object o) {
        int i = 0;
        boolean found = false;
        while (i < this._length && !found) {
            if (o.equals(this._bits[i])) {
                found = true;
            }
            ++i;
        }
        return found;
    }

    /**
     * Return an iterator over the bits of the bitarray instance in proper
     * sequence
     * @return Iterator over the bits of the bitarray
     */
    @NotNull public Iterator<Boolean> iterator() {
        return new BitArrayIterator(this);
    }

    /**
     * Return a Boolean array corresponding to the bitarray instance.
     * @return Corresponding Boolean array
     */
    @NotNull public Boolean[] toArray() {
        Boolean[] result = new Boolean[this._length];
        System.arraycopy(this._bits, 0, result, 0, this._length);
        return result;
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    @NotNull public <T> T[] toArray(@NotNull T[] a) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Add a bit at the end of the bitarray. Almost same as {@link
     * BitArray#push push} method.
     * @param bit
     *     Bit to be added at the end of the bitarray
     * @return true
     */
    public boolean add(Boolean bit) {
        this.add(this._length, bit);
        return true;
    }

    /**
     * Remove the first occurrence of o found in the bitarray. If o is not
     * found, do nothing.
     * @param o
     *     Object to remove from the bitarray
     * @return true if the specified object was found, false otherwise
     */
    public boolean remove(Object o) {
        boolean found = false;
        int index = this.indexOf(o);
        if (index != -1) {
            this.remove(index);
            found = true;
        }
        return found;
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    public boolean containsAll(@NotNull Collection<?> c) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    public boolean addAll(@NotNull Collection<? extends Boolean> c) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    public boolean addAll(
        int index, @NotNull Collection<? extends Boolean> c) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    public boolean removeAll(@NotNull Collection<?> c) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    public boolean retainAll(@NotNull Collection<?> c) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    public void replaceAll(UnaryOperator<Boolean> operator) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Unsupported operation. Used to implement List only.
     */
    public void sort(Comparator<? super Boolean> c) {
        throw new UnsupportedOperationException("Invalid operation for this " +
                                                "class");
    }

    /**
     * Removes all bit from the bitarray.
     */
    public void clear() {
        this._length = 0;
    }

    /**
     * Get the bit value at index i.
     * @param i
     *     Index of the bit to be returned
     * @return Bit at index i
     * @throws java.lang.IndexOutOfBoundsException
     *     if index is not valid
     */
    public Boolean get(int i) throws IndexOutOfBoundsException {
        if (i < 0 || i >= this._length) {
            throw new IndexOutOfBoundsException("Invalid index.");
        }
        return this._bits[i];
    }

    /**
     * Set the bit at index i to the given value. Be careful! i must be
     * strictly lower than length of the bitarray.
     * @param i
     *     Index of the bit to be modified
     * @param bit
     *     New value for this bit
     * @throws java.lang.IndexOutOfBoundsException
     *     if index is not valid
     */
    public Boolean set(int i, Boolean bit) throws IndexOutOfBoundsException {
        if (i < 0 || i >= this._length) {
            throw new IndexOutOfBoundsException("Invalid index.");
        }
        Boolean prevBit = this._bits[i];
        this._bits[i] = bit;
        return prevBit;
    }

    /**
     * Insert the specified bit at the specified index in the bitarray and
     * shift all following bits.
     * @param index
     *     Index where the given bit is inserted
     * @param bit
     *     Bit to be inserted
     * @throws java.lang.IndexOutOfBoundsException
     *     if index is not valid
     */
    public void add(int index, Boolean bit)
        throws IndexOutOfBoundsException, NullPointerException {
        if (bit == null) {
            throw new NullPointerException("The specified bit is null.");
        }
        if (index < 0 || index > this._length) {
            throw new IndexOutOfBoundsException("Invalid index.");
        }
        if (this._length < this._space) {
            if (this._readPosition > index) {
                this._readPosition = 0;
            }
            this._bits[this._length] = BitArray.DEFAULT_VALUE;
            Boolean nextValue = this._bits[index];
            Boolean temp;
            for (int i = index; i < this._length; ++i) {
                temp = this._bits[i + 1];
                this._bits[i + 1] = nextValue;
                nextValue = temp;
            }
            this._bits[index] = bit;
            ++this._length;
        } else {
            try {
                this.addSpace();
            } catch (NegativeSpaceBitArrayException |
                NotEnoughSpaceBitArrayException e) {
                throw new IndexOutOfBoundsException(e.getMessage());
            }
            this.add(index, bit);
        }
    }

    /**
     * Remove the bit at the specified position in the bitarray.
     * @param index
     *     Index where the bit has to be removed
     * @return Value of the removed bit
     */
    @SuppressWarnings("ManualArrayCopy") public Boolean remove(int index)
        throws IndexOutOfBoundsException {
        if (index < 0 || index >= this._length) {
            throw new IndexOutOfBoundsException("Invalid index.");
        }
        Boolean prevBit = this._bits[index];
        for (int i = index + 1; i < this._length; ++i) {
            this._bits[i - 1] = this._bits[i];
        }
        --this._length;
        return prevBit;
    }

    /**
     * Return the position where the first occurrence of o was met, and -1
     * otherwise.
     * @param o
     *     Object to find in the bitarray
     * @return Index of the first occurrence of o, -1 if o was not found
     */
    public int indexOf(Object o) {
        int index = -1;
        int i = 0;
        while (i < this._length && index == -1) {
            if (o.equals(this._bits[i])) {
                index = i;
            }
            ++i;
        }
        return index;
    }

    /**
     * Return the position where the last occurrence of o was met, and -1
     * otherwise
     * @param o
     *     Object to be found in the bitarray
     * @return Index of the first occurrence of o, -1 if o was not found
     */
    public int lastIndexOf(Object o) {
        int i = 0;
        int index = -1;
        while (i < this._length) {
            if (o.equals(this._bits[i])) {
                index = i;
            }
            ++i;
        }
        return index;
    }

    /**
     * Return a list iterator over the bits of the bitarray instance in proper
     * sequence.
     * @return List iterator over the bits of the bitarray
     */
    @NotNull public ListIterator<Boolean> listIterator() {
        return new BitArrayListIterator(this);
    }

    /**
     * Return a list iterator over the bits of the bitarray instance starting
     * at the specified index, in proper sequence.
     * @param i
     *     Starting index for the reading
     * @return List iterator over the bits of the bitarray
     * @throws java.lang.IndexOutOfBoundsException
     *     if index is not valid
     */
    @NotNull public ListIterator<Boolean> listIterator(int i) {
        return new BitArrayListIterator(this, i);
    }

    /**
     * Return a new bitarray containing all bits from fromIndex index
     * (included) to toIndex index (excluded).
     * @param fromIndex
     *     Starting index (included)
     * @param toIndex
     *     Ending index (excluded)
     * @return New sub-bitarray
     * @throws java.lang.IndexOutOfBoundsException
     *     if indexes are not valid
     */
    @NotNull public List<Boolean> subList(int fromIndex, int toIndex)
        throws IndexOutOfBoundsException {
        if (fromIndex < 0 ||
            fromIndex >= this._length ||
            toIndex < 0 ||
            toIndex > this._length ||
            toIndex < fromIndex) {
            throw new IndexOutOfBoundsException("Invalid indexes.");
        }
        int newLength = toIndex - fromIndex;
        BitArray subBitarray;
        try {
            subBitarray =
                new BitArray((int) (BitArray.DEFAULT_SPACE_INIT_PERCENT *
                                    newLength));
        } catch (NegativeSpaceBitArrayException e) {
            throw new IndexOutOfBoundsException(e.getMessage());
        }
        System.arraycopy(this._bits,
                         fromIndex,
                         subBitarray._bits,
                         0,
                         newLength);
        subBitarray._length = newLength;
        return subBitarray;
    }

    /**
     * Convert the bitarray to its decimal value.
     * @return Decimal value corresponding to the bitarray
     * @throws TooLargeDecimalValueBitArrayException
     *     if the bitarray holds more than 64 bits
     */
    public long toDecimal() throws TooLargeDecimalValueBitArrayException {
        if (this._length > 64) {
            throw new TooLargeDecimalValueBitArrayException("This bitarray " +
                                                            "holds too many " +
                                                            "bits to be " +
                                                            "converted to " +
                                                            "long.");
        }
        int value = 0;
        for (int i = 0; i < this._length; ++i) {
            value *= 2;
            if (this._bits[i]) {
                ++value;
            }
        }
        return value;
    }
}

