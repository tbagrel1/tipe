package org.tbagrel.treeutil.adaptativehuffmantree.exceptions;

/**
 * Exception meaning that the specified value wasn't found in the requested
 * map.
 */
public class ValueNotFoundAHTException extends Exception {
    /**
     * Default constructor.
     */
    public ValueNotFoundAHTException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public ValueNotFoundAHTException(String message) {
        super(message);
    }
}
