package org.tbagrel.bitutil.bitarray.exceptions;

/**
 * Exception meaning that the current array holds too many bits to be converted
 * to long.
 */
public class TooLargeDecimalValueBitArrayException extends Exception {
    /**
     * Default constructor.
     */
    public TooLargeDecimalValueBitArrayException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public TooLargeDecimalValueBitArrayException(String message) {
        super(message);
    }
}
