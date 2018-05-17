package org.tbagrel.treeutil.adaptativehuffmantree.exceptions;

/**
 * Exception meaning that the code map of the AHT is not valid; eg, because all
 * bitarrays don't have the same length.
 */
public class InvalidCodeMapAHTException extends Exception {
    /**
     * Default constructor.
     */
    public InvalidCodeMapAHTException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public InvalidCodeMapAHTException(String message) {
        super(message);
    }
}
