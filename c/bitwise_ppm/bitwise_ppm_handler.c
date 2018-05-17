#include "bitwise_ppm_handler.h"

#include <stdio.h>

// TODO: Free

#define ACODE_T_BUFFER_SIZE \
    (BUFFER_SIZE / (sizeof(acode_t) / (sizeof(uint8_t))))

/**
 * Compress the file specified by raw_path to the file specified by enc_path
 * using the bitwise_ppm algorithm
 * @param raw_path path to the input (no compressed) file
 * @param enc_path path to the output (compressed) file
 */
void encode_file(
    str raw_path, str enc_path, prob_t *escape_probs) {
    FILE *raw_file = fopen(raw_path, "rb");

    ModelHolder *model_enc = modelholder__new();
    modelholder__init(model_enc, escape_probs);
    ArithmeticCodingHolder *acoder = arithmeticcoding__new();

    uint8_t buffer[BUFFER_SIZE];
    size_t bytes_read;
    uint8_t byte;
    bit_t bit;
    size_t i_byte;
    size_t i_bit;
    context_t context = 0;
    do {
        bytes_read = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, raw_file);
        for (i_byte = 0; i_byte < bytes_read; i_byte++) {
            byte = buffer[i_byte];
            for (i_bit = 0; i_bit < BYTE_SIZE; i_bit++) {
                bit = (byte >> (BYTE_SIZE - 1 - i_bit)) & 1U;
                encode(acoder, model_enc, context, bit);
                context = (context << 1) | bit;
            }
        }
    } while (bytes_read == BUFFER_SIZE);

    fclose(raw_file);

    acode_t_vect *enc_vect = arithmeticcoding__to_out_format(acoder);

    FILE *enc_file = fopen(enc_path, "wb");

    fwrite(enc_vect->_vector, sizeof(acode_t), enc_vect->_size, enc_file);

    fclose(enc_file);
}

/**
 * Decompress the file specified by enc_path to the file specified by raw_path
 * using the bitwise_ppm algorithm
 * @param enc_path path to the input (compressed) file
 * @param raw_path path to the output (decompressed) file
 */
void decode_file(
    str enc_path, str raw_path, prob_t *escape_probs) {
    FILE *enc_file = fopen(enc_path, "rb");

    ModelHolder *model_dec = modelholder__new();
    modelholder__init(model_dec, escape_probs);

    acode_t_vect *enc_vect = acode_t_vect__new();

    acode_t acode_t_buffer[ACODE_T_BUFFER_SIZE];
    size_t acode_ts_read;
    do {
        acode_ts_read = fread(
            acode_t_buffer, sizeof(acode_t), ACODE_T_BUFFER_SIZE, enc_file);
        for (size_t i_acode_t = 0; i_acode_t < acode_ts_read; i_acode_t++) {
            acode_t_vect__add(enc_vect, acode_t_buffer[i_acode_t]);
        }
    } while (acode_ts_read == ACODE_T_BUFFER_SIZE);

    fclose(enc_file);

    ArithmeticCodingHolder *acoder =
        arithmeticcoding__from_out_format(enc_vect);

    FILE *raw_file = fopen(raw_path, "wb");

    uint8_t byte = 0;
    context_t context = 0;
    bit_t bit;
    size_t bytes_in_buffer = 0;
    size_t bits_in_byte = 0;
    uint8_t buffer[BUFFER_SIZE];
    while ((bit = decode(acoder, model_dec, context), bit) != BIT_STOP) {
        context = (context << 1) | bit;
        if (bit == BIT_ERROR) {
        }
        byte |= (bit << (BYTE_SIZE - 1 - (bits_in_byte++)));
        if (bits_in_byte == BYTE_SIZE) {
            buffer[bytes_in_buffer++] = byte;
            byte = 0;
            bits_in_byte = 0;
        }
        if (bytes_in_buffer == BUFFER_SIZE) {
            fwrite(buffer, sizeof(uint8_t), BUFFER_SIZE, raw_file);
            bytes_in_buffer = 0;
        }
    }
    if (bits_in_byte > 0) {
        printf("[Warning] Not completed byte\n");
        buffer[bytes_in_buffer++] = byte;
    }
    if (bytes_in_buffer > 0) {
        fwrite(buffer, sizeof(uint8_t), bytes_in_buffer, raw_file);
    }

    fclose(raw_file);
}
