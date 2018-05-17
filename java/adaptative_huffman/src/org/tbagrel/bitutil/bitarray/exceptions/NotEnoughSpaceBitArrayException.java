package org.tbagrel.bitutil.bitarray.exceptions;

/**
 * Exception meaning that the specified space is not large enough to copy the
 * specified bitarray into the new one.
 */
public class NotEnoughSpaceBitArrayException extends Exception {
    /**
     * Default constructor.
     */
    public NotEnoughSpaceBitArrayException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public NotEnoughSpaceBitArrayException(String message) {
        super(message);
    }
}
