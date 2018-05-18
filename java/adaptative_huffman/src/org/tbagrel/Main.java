package org.tbagrel;

import org.tbagrel.bitutil.bitarray.BitArray;
import org.tbagrel.bitutil.bitarray.exceptions.*;
import org.tbagrel.treeutil.adaptativehuffmantree.AdaptativeHuffmanTree;
import org.tbagrel.treeutil.adaptativehuffmantree.exceptions.*;
import org.tbagrel.treeutil.binode.exceptions.InvalidPositionBiNodeException;

import java.util.*;
import java.nio.file.*;

/**
 * Class for tests
 */
@SuppressWarnings("SpellCheckingInspection") public
class Main {

    /**
     * Size of the default code for chars.
     */
    public static final int BYTE_SIZE = 8;

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
                             InvalidCodeMapAHTException,
                             java.io.IOException {
        System.out.println(new java.io.File(".").getCanonicalPath());
        HashMap<Character, BitArray> myMap = new HashMap<>();
        for (int i = 0; i < 256; ++i) {
            myMap.put((char) (i), BitArray.fromCustom(i, BYTE_SIZE));
        }
        AdaptativeHuffmanTree<Character>
            myEncodingTree =
            new AdaptativeHuffmanTree<>(null, (char) (256), myMap);
        AdaptativeHuffmanTree<Character>
            myDecodingTree =
            new AdaptativeHuffmanTree<>(null, (char) (256), myMap);

        byte[] myBytes = Files.readAllBytes(Paths.get("./calgary.tar"));

        int initialSize = myBytes.length * Main.BYTE_SIZE;
        // System.out.prCharacterln(myString);

        List<Character> inputData = new ArrayList<>();
        for (int i = 0; i < myBytes.length; ++i) {
            inputData.add((char) (myBytes[i] + 128));
        }

        BitArray encodedData = myEncodingTree.encode(inputData);
        int finalSize = encodedData.getLength();

        List<Character> decodedData = myDecodingTree.decode(encodedData);

        byte[] decData = new byte[decodedData.size()];
        for (int i = 0; i < decodedData.size(); i++) {
            decData[i] = (byte) (decodedData.get(i) - 128);
        }

        Files.write(Paths.get("./calgary.dec"), decData);

        double
            compressionRatio =
            1.0 - ((double) finalSize) / ((double) initialSize);
        System.out.println(compressionRatio);
    }
}
