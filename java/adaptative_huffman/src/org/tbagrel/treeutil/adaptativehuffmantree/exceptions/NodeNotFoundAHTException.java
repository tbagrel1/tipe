package org.tbagrel.treeutil.adaptativehuffmantree.exceptions;

/**
 * Exception meaning that no node was found with the specified parameters.
 */
public class NodeNotFoundAHTException extends Exception {
    /**
     * Default constructor.
     */
    public NodeNotFoundAHTException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public NodeNotFoundAHTException(String message) {
        super(message);
    }
}