package org.tbagrel.treeutil.binode.exceptions;

/**
 * Exception meaning that the position of the binode is not correct.
 */
public class InvalidPositionBiNodeException extends Exception {
    /**
     * Default constructor.
     */
    public InvalidPositionBiNodeException() {
        super();
    }

    /**
     * Constructor with a message to be printed.
     * @param message
     *     Message to be printed
     */
    public InvalidPositionBiNodeException(String message) {
        super(message);
    }
}
