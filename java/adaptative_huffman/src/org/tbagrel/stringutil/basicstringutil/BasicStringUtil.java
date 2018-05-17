package org.tbagrel.stringutil.basicstringutil;

import org.tbagrel.stringutil.basicstringutil.exceptions
    .InvalidLengthBSUException;

/**
 * Set of basic static method on strings.
 */
public class BasicStringUtil {
    /**
     * Return a representation of the specified object centered in a string
     * containing length characters.
     * @param o
     *     Object to be represented
     * @param length
     *     Total length of the centered representation string
     * @return Centered representation of the specified object
     * @throws InvalidLengthBSUException
     *     if the specified length is not large enough to hold the specified
     *     object representation
     */
    public static String centered(Object o, int length)
        throws InvalidLengthBSUException {
        String core = o.toString();
        if (core.length() > length) {
            throw new InvalidLengthBSUException("Specified length is not " +
                                                "large enough to hold the " +
                                                "specified object " +
                                                "representation");
        }
        int space = length - core.length();
        return BasicStringUtil.mult(" ", space - space / 2) +
               core +
               BasicStringUtil.mult(" ", space / 2);
    }

    /**
     * Return the specified String multiplied by k.
     * @param str
     *     String to be multiplied
     * @param k
     *     Multiplicative factor
     * @return Multiplied String.
     */
    public static String mult(String str, int k) {
        return new String(new char[k]).replace("\0", str);
    }

    /**
     * Compute the length of the specified object representation. Specified
     * object might be null pointer, in that case, return 0.
     * @param o
     *     Object from which length is computed
     * @return Length of the specified object representation
     */
    public static int safeLength(Object o) {
        int length;
        try {
            length = o.toString().length();
        } catch (NullPointerException e) {
            length = 0;
        }
        return length;
    }
}
