package org.tbagrel.stringutil.basicstringutil.exceptions;

/**
 * Exception meaning that the specified length is not large enough to store the
 * specified object representation
 */
public class InvalidLengthBSUException extends Exception {
    /**
     * Default constructor.
     */
    public InvalidLengthBSUException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public InvalidLengthBSUException(String message) {
        super(message);
    }
}
