package org.tbagrel;

import org.tbagrel.bitutil.bitarray.BitArray;
import org.tbagrel.bitutil.bitarray.exceptions.*;
import org.tbagrel.treeutil.adaptativehuffmantree.AdaptativeHuffmanTree;
import org.tbagrel.treeutil.adaptativehuffmantree.exceptions.*;
import org.tbagrel.treeutil.binode.exceptions.InvalidPositionBiNodeException;

import java.util.*;

/**
 * Class for tests
 */
@SuppressWarnings("SpellCheckingInspection") public
class Main {

    /**
     * Size of the default code for chars.
     */
    public static final int CHAR_SIZE = 7;

    /**
     * Main static method
     *
     * @param args Command-line args
     */
    public static
    void main(String[] args) throws
                             NegativeSpaceBitArrayException,
                             NegativeDecimalValueBitArrayException,
                             NodeNotFoundAHTException,
                             InvalidPositionBiNodeException,
                             ValueNotFoundAHTException,
                             NotEnoughSpaceBitArrayException,
                             InvalidCodeMapAHTException {
        HashMap<Character, BitArray> myMap = new HashMap<>();
        for (long i = 0; i < 128; ++i) {
            myMap.put((char) i, BitArray.fromCustom(i, CHAR_SIZE));
        }
        AdaptativeHuffmanTree<Character>
            myEncodingTree =
            new AdaptativeHuffmanTree<>(null, (char) 128, myMap);
        AdaptativeHuffmanTree<Character>
            myDecodingTree =
            new AdaptativeHuffmanTree<>(null, (char) 128, myMap);

        String myString = new String(
            Files.readAllBytes(Paths.get("corpora.txt")),
            StandardCharsets.ISO_8859_1);

        int initialSize = myString.length() * Main.CHAR_SIZE;
        // System.out.println(myString);

        List<Character> inputData = new ArrayList<>();
        for (int i = 0; i < myString.length(); ++i) {
            inputData.add(myString.charAt(i));
        }

        BitArray encodedData = myEncodingTree.encode(inputData);
        int finalSize = encodedData.getLength();

        List<Character> decodedData = myDecodingTree.decode(encodedData);

        StringBuilder myDecodedStringBuilder = new StringBuilder();
        for (Character c : decodedData) {
            myDecodedStringBuilder.append(c);
        }
        String myDecodedString = myDecodedStringBuilder.toString();
        // System.out.println(myDecodedString);

        double
            compressionRatio =
            1.0 - ((double) finalSize) / ((double) initialSize);
        System.out.println(compressionRatio);
    }
}
