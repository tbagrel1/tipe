package org.tbagrel.bitutil.bitarray.exceptions;

/**
 * Exception meaning that a strictly negative decimal value was specified for
 * conversion to bitarray.
 */
public class NegativeDecimalValueBitArrayException extends Exception {
    /**
     * Default constructor
     */
    public NegativeDecimalValueBitArrayException() {
        super();
    }

    /**
     * Constructor with a message to be printed
     * @param message
     *     Message to be printed
     */
    public NegativeDecimalValueBitArrayException(String message) {
        super(message);
    }
}
