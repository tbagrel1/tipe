package org.tbagrel.bitutil.bitarray.exceptions;

/**
 * Exception meaning that a strictly negative space was specified during
 * bitarray construction.
 */
public class NegativeSpaceBitArrayException extends Exception {
    /**
     * Default constructor.
     */
    public NegativeSpaceBitArrayException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public NegativeSpaceBitArrayException(String message) {
        super(message);
    }
}
