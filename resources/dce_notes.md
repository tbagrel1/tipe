Col... ? corpora / corpus
Generic Compression Benchmark
maximumcompression.com

Boutayna faire du Français et seulement ça en Français à partir du 18.01 et jusqu'aux vacances.

>>> nanozip / paq8, freearc, ccm, flagship, 7-zip

arrêt 3.3


4.1.2. Bitwise encoding
The idea is to encode one bit at a time by using the previous bits of the current byte as additional context. Only two values are stored: a count of ones, count1, and a total count. The prediction is count1/count. The update procedure is to increment count and to increment count1 if the bit is 1. We handle zero probabilities, overflow, and large contexts as before.

Alternatively, we can avoid a (slow) division operation by storing the prediction directly. Each bitwise context is associated with a prediction that the next bit will be a 1 (initially 1/2) and an update count (initially 0). The update rate is initially fast and decreases as the count is incremented, resulting in a stationary model. Alternatively, the count can be bounded, resulting in an adaptive model.

    // Prediction and count for one bitwise context
    struct Model {
      double prediction;  // between 0 and 1 that next bit will be a 1
      int count;          // number of updates
      Model(): prediction(0.5), count(0) {}
    };
  
    Model model[CONTEXT_SIZE][256]; // context, bit_context -> prediction and count
    int context = 0;                // bytewise order n context
  
    // Compress byte c in MSB to LSB order
    void compress(int c) {
      for (int i=7; i>=0 --i) {
        int bit_context = c+256 >> i+1;
        int bit = (c >> i) % 2;
        encode(bit, model[context][bit_context].prediction);
        update(bit, model[context][bit_context]);
      }
      context = (context << 8 | c) % CONTEXT_SIZE;
    }
  
    // Decompress and return a byte
    int decompress() {
      int c;  // bit_context
      int bit;
      for (c = 1; c < 256; c = c * 2 + bit) {
        bit = decode(model[context][c].prediction);
        update(bit, model[context][c]);
      }
      c -= 256;  // decoded byte
      context = (context << 8 | c) % CONTEXT_SIZE;
      return c;
    }
  
    // Update the model
    void update(int bit, Model& m) {
      const double DELTA = 0.5;
      const int LIMIT = 255;
      if (m.count < LIMIT) ++m.count;
      m.prediction += (bit - m.prediction) / (m.count + DELTA);
    }
The compress() function takes a byte c and compresses it one bit at a time starting with the most significant bit. At each of the 8 steps, the previously coded bits are packed into a number in the range (1..255) as a binary number 1 followed by up to 7 earlier bits. For example, if c = 00011100, then bit_context takes the 8 successive values 1, 10, 100, 1000, 10001, 100011, 1000111, 10001110. In decompress(), c plays the same role. After 8 decoding operations it has the value 100011100 and the leading 1 is stripped off before being returned.

As before, the context may also be a hash.

The update function computes the prediction error (bit - m.prediction) and adjusts the prediction in inverse proportion to the count. The count is incremented up to a maximum value. At this point, the model switches from stationary to adaptive.

DELTA and LIMIT are tunable parameters. The best values depend on the data. A large LIMIT works best for stationary data. A smaller LIMIT works better for mixed data types. On stationary sources, the compressed size is typically larger by 1/LIMIT. The choice of DELTA is less critical because it only has a large effect when the data size is small (relative to the model size). With DELTA = 1, a series of zero bits would result in the prediction sequence 1/2, 1/4, 1/6, 1/8, 1/10. With DELTA = 0.5, the sequence would be 1/2, 1/6, 1/10, 1/14, 1/18. Cleary and Teahan (1995) measured the actual probabilities in English text and found a sequence near 1/2, 1/30, 1/60, 1/90... for zeros and 1/2, 19/20, 39/40, 59/60... for consecutive ones. This would fit DELTA around 0.07 to 0.1.

A real implementation would use integer arithmetic to represent fixed point numbers, and use a lookup table to compute 1/(m.count + DELTA) in update() to avoid a slow division operation. ZPAQ packs a 22 bit prediction and 10 bit count into a 32 bit model element. As a further optimization, the model is stored as a one dimensional array aligned on a 64 byte cache line boundary. The bytewise context is updated once per byte as usual, but the extra bits are expanded in groups of 4 in a way that causes only two cache misses per byte. The leading bits are expanded to 9 bits as shown below, then exclusive-ORed with the bytewise context address.

    0 0000 0001
    0 0000 001x
    0 0000 01xx
    0 0000 1xxx
    1 xxxx 0001
    1 xxxx 001x
    1 xxxx 01xx
    1 xxxx 1xxx
ZPAQ fixes DELTA at 1/2 but LIMIT is configurable to 4, 8, 12,..., 1020. The following table shows the effect of varying LIMIT for an order 0 model on 106 digits of π (stationary) and orders 0 through 2 on the 14 file Calgary corpus concatenated into a single data stream (nonstationary). Using a higher order model can improve compression at the cost of memory. However, direct lookup tables are not practical for orders higher than about 2. The order 2 model in ZPAQ uses 134 MB memory. The higher orders have no effect on π because the digits are independent (short of actually computing π).


            pi              Calgary corpus
    LIMIT order-0     order-0   order-1   order-2
    ----- -------    --------- --------- ---------
       4  455,976    1,859,853 1,408,402 1,153,855
       8  435,664    1,756,081 1,334,979 1,105,621
      16  425,490    1,704,809 1,306,838 1,089,660 
      32  420,425    1,683,890 1,304,204 1,091,029
      64  417,882    1,680,784 1,315,988 1,101,612
     128  416,619    1,686,478 1,335,080 1,115,717
     256  415,990    1,696,658 1,357,396 1,129,790
     512  415,693    1,710,035 1,379,823 1,141,800
    1020  415,566    1,726,280 1,399,988 1,150,737

    4.3.7


Intéressant : http://marknelson.us/1991/02/01/arithmetic-coding-statistical-modeling-data-compression/