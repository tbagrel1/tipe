package org.tbagrel.textutil;

import org.tbagrel.bitutil.bitarray.*;

/**
 * Class used to store a dict of string -> code & code -> string.
 */
public class BagrelCodeDict {

    public static final long STARTING_INDEX = 128;

    /**
     * Dict storing string -> code
     */
    protected java.util.HashMap<String, BitArray> _stringToCode;

    /**
     * Dict storing code -> string
     */
    protected java.util.HashMap<BitArray, String> _codeToString;

    /**
     * Storing the length of each code
     */
    protected int _codeLength;

    /**
     * Default constructor.
     * @param values List of string values to add to the standard ASCII table
     */
    public BagrelCodeDict(java.util.List<String> values) {
        this._codeLength = (int)(Math.log((double)(org.tbagrel.textutil
            .BagrelCodeDict.STARTING_INDEX + values.size())) / Math.log(
                (double)(2)));
    }
}
