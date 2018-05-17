package org.tbagrel.textutil;

public class Bagrel1C {
    /**
     * Class used to compress text by using n-bits sequences to represent
     * single characters or pairs of characters.
     */

    /**
     * Dict used to store special code values of pairs.
     */
    protected java.util.Map<String, Long> _codes;

    /**
     * List of all values used for pairs.
     */
    protected java.util.List<String> _used_for_pairs;

    /**
     * Default constructor.
     */
    public Bagrel1C() {

    }
}
