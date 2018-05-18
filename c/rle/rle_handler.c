#include "rle_handler.h"

size_t _gcd(size_t a, size_t b) {
    if (b == 0) {
        return a;
    }
    return _gcd(b, a % b);
}

size_t _lcm(size_t a, size_t b) {
    return (a * b) / _gcd(a, b);
}

size_t lcm(size_t a, size_t b) {
    return (a > b) ? _lcm(a, b) : _lcm(b, a);
}

void buffer_write_at(
    byte_t *buffer, size_t i_bit, rle_code_t rle_code, size_t rle_code_size) {
    size_t i_byte = i_bit / BYTE_SIZE;
    size_t bit_loffset = i_bit % BYTE_SIZE;
    size_t bit_roffset = BYTE_SIZE - bit_loffset;
    size_t remaining_bits = rle_code_size;
    buffer[i_byte] =
        ((buffer[i_byte] >> bit_roffset) << bit_roffset) |
        (rle_code >> (rle_code_size - bit_roffset));
    remaining_bits -= bit_roffset;
    i_byte += 1;
    size_t mask = (1U << BYTE_SIZE) - 1;
    while (remaining_bits > 0) {
        bit_loffset =
            (remaining_bits >= BYTE_SIZE) ? BYTE_SIZE : remaining_bits;
        bit_roffset = BYTE_SIZE - bit_loffset;
        buffer[i_byte] =
            (rle_code >> (remaining_bits - bit_loffset)) << bit_roffset;
        remaining_bits -= bit_loffset;
        i_byte += 1;
    }
}

rle_code_t buffer_read_at(byte_t *buffer, size_t i_bit, size_t rle_code_size) {
    rle_code_t rle_code = 0;
    size_t i_byte = i_bit / BYTE_SIZE;
    size_t bit_loffset = i_bit % BYTE_SIZE;
    size_t bit_roffset = BYTE_SIZE - bit_loffset;
    size_t remaining_bits = rle_code_size;
    rle_code = buffer[i_byte] & ((1U << bit_roffset) - 1);
    remaining_bits -= bit_roffset;
    i_byte += 1;
    while (remaining_bits > 0) {
        bit_loffset =
            (remaining_bits >= BYTE_SIZE) ? BYTE_SIZE : remaining_bits;
        bit_roffset = BYTE_SIZE - bit_loffset;
        rle_code = (rle_code << bit_loffset) | (buffer[i_byte] >> bit_roffset);
        remaining_bits -= bit_loffset;
        i_byte += 1;
    }
    return rle_code;
}

void encode_file(
    str raw_path, str enc_path, size_t count_bits) {
    FILE *raw_file = fopen(raw_path, "rb");

    byte_t buffer[BUFFER_SIZE];
    size_t bytes_read;
    byte_t byte, prev_byte;
    size_t total_i_byte = 0;
    uint16_t count = 0;
    uint16_t max_count = 1U << count_bits;
    size_t i_byte;
    rle_code_t rle_code;

    rle_code_t_vect *rle_codes = rle_code_t_vect__new();

    do {
        bytes_read = fread(buffer, sizeof(byte_t), BUFFER_SIZE, raw_file);
        if (total_i_byte == 0) {
            prev_byte = buffer[0];
        }
        for (i_byte = 0; i_byte < bytes_read; i_byte++, total_i_byte++) {
            byte = buffer[i_byte];
            if (byte != prev_byte || count == max_count) {
                rle_code = (count - 1U) << BYTE_SIZE | prev_byte;
                rle_code_t_vect__add(rle_codes, rle_code);
                count = 1;
                prev_byte = byte;
            } else {
                count++;
            }
        }
    } while (bytes_read == BUFFER_SIZE);

    fclose(raw_file);

    FILE *enc_file = fopen(enc_path, "wb");

    rle_code_t_vect__add(rle_codes, (count - 1U) << BYTE_SIZE | prev_byte);

    size_t rle_code_size = count_bits + BYTE_SIZE;
    size_t bits_in_wbuffer = lcm(rle_code_size, BYTE_SIZE);
    size_t bytes_in_wbuffer = bits_in_wbuffer / BYTE_SIZE;
    size_t rle_codes_in_wbuffer = bits_in_wbuffer / rle_code_size;

    byte_t rle_codes_in_last = rle_codes->_size % rle_codes_in_wbuffer;
    size_t wbuffer_nb = rle_codes->_size / rle_codes_in_wbuffer;

    byte_t *wbuffer = (byte_t *) (malloc(sizeof(byte_t) * bytes_in_wbuffer));
    size_t i_wbuffer, i_bit, i_rle_code;

    fwrite(&rle_codes_in_last, sizeof(byte_t), 1, raw_file);

    for (i_wbuffer = 0; i_wbuffer < wbuffer_nb; i_wbuffer++) {
        i_bit = 0;
        wbuffer[0] = 0;
        for (i_rle_code = 0; i_rle_code < rle_codes_in_wbuffer; i_rle_code++) {
            rle_code = rle_code_t_vect__get(
                rle_codes, i_wbuffer * rle_codes_in_wbuffer + i_rle_code);
            buffer_write_at(wbuffer, i_bit, rle_code, rle_code_size);
            i_bit += rle_code_size;
        }
        fwrite(wbuffer, sizeof(byte_t), bytes_in_wbuffer, enc_file);
    }

    i_bit = 0;
    wbuffer[0] = 0;
    for (i_rle_code = 0; i_rle_code < rle_codes_in_last; i_rle_code++) {
        rle_code = rle_code_t_vect__get(
            rle_codes, wbuffer_nb * rle_codes_in_wbuffer + i_rle_code);
        buffer_write_at(wbuffer, i_bit, rle_code, rle_code_size);
        i_bit += rle_code_size;
    }
    fwrite(
        wbuffer, sizeof(byte_t), (size_t) (
            ceil((double) (rle_codes_in_last * rle_code_size) / BYTE_SIZE)),
        enc_file);

    fclose(enc_file);
}

void decode_file(str enc_path, str raw_path, size_t count_bits) {

    FILE *enc_file = fopen(enc_path, "rb");

    size_t rle_code_size = count_bits + BYTE_SIZE;
    size_t bits_in_wbuffer = lcm(rle_code_size, BYTE_SIZE);
    size_t bytes_in_wbuffer = bits_in_wbuffer / BYTE_SIZE;
    size_t rle_codes_in_wbuffer = bits_in_wbuffer / rle_code_size;

    byte_t *rbuffer = (byte_t *) (malloc(sizeof(byte_t) * bytes_in_wbuffer));

    rle_code_t_vect *rle_codes = rle_code_t_vect__new();

    byte_t rle_codes_in_last;
    fread(&rle_codes_in_last, sizeof(byte_t), 1, enc_file);

    size_t bytes_read;
    size_t i_bit;
    size_t i_rle_code;
    while ((
               bytes_read = fread(
                   rbuffer, sizeof(byte_t), bytes_in_wbuffer,
                   enc_file), bytes_read) == bytes_in_wbuffer) {
        i_bit = 0;
        for (i_rle_code = 0; i_rle_code < rle_codes_in_wbuffer; i_rle_code++) {
            rle_code_t_vect__add(
                rle_codes, buffer_read_at(rbuffer, i_bit, rle_code_size));
            i_bit += rle_code_size;
        }
    }

    i_bit = 0;
    for (i_rle_code = 0; i_rle_code < rle_codes_in_last; i_rle_code++) {
        rle_code_t_vect__add(
            rle_codes, buffer_read_at(rbuffer, i_bit, rle_code_size));
        i_bit += rle_code_size;
    }

    fclose(enc_file);

    FILE *raw_file = fopen(raw_path, "wb");

    byte_t buffer[BUFFER_SIZE];
    size_t bytes_written = 0;
    size_t mask = (1U << BYTE_SIZE) - 1;
    count_t count, i_count;
    byte_t byte;
    rle_code_t rle_code;

    for (i_rle_code = 0; i_rle_code < rle_codes->_size; i_rle_code++) {
        rle_code = rle_code_t_vect__get(rle_codes, i_rle_code);
        count = (rle_code >> BYTE_SIZE) + 1;
        byte = rle_code & mask;
        for (i_count = 0; i_count < count; i_count++) {
            if (bytes_written == BUFFER_SIZE) {
                fwrite(buffer, sizeof(byte_t), bytes_written, raw_file);
                bytes_written = 0;
            }
            buffer[bytes_written++] = byte;
        }
    }
    if (bytes_written > 0) {
        fwrite(buffer, sizeof(byte_t), bytes_written, raw_file);
    }

    fclose(raw_file);
}
