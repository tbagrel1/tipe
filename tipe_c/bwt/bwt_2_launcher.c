#include <stdio.h>
#include <stdlib.h>
#include "bwt_2_launcher.h"

int bwt_code(str path_in, str path_out) {
    FILE *file_in = NULL;
    FILE *file_out = NULL;
    file_in = fopen(path_in, "rb");
    file_out = fopen(path_out, "wb");
    if (file_in == NULL || file_out == NULL) {
        return 1;
    }
    // ------------------------------------------------------------------------
    uchar buffer[BLOCK_SIZE];
    str coded_data;
    size_t bytes_read;
    do {
        bytes_read = fread(buffer, sizeof(*buffer), BLOCK_SIZE, file_in);
        if (bytes_read > 0) {
            coded_data = bwt_code_block(buffer, bytes_read);
            fwrite(
                coded_data, sizeof(*coded_data), INDEX_SIZE + bytes_read,
                file_out);
            free(coded_data);
        }
    } while (bytes_read == BLOCK_SIZE);
    // ------------------------------------------------------------------------
    fclose(file_out);
    fclose(file_in);
    return 0;
}

int bwt_decode(str path_in, str path_out) {
    FILE *file_in = NULL;
    FILE *file_out = NULL;
    file_in = fopen(path_in, "rb");
    file_out = fopen(path_out, "wb");
    if (file_in == NULL || file_out == NULL) {
        return 1;
    }
    // ------------------------------------------------------------------------
    uchar buffer[INDEX_SIZE + BLOCK_SIZE];
    str decoded_data;
    size_t bytes_read;
    do {
        bytes_read = fread(
            buffer, sizeof(*buffer), INDEX_SIZE + BLOCK_SIZE, file_in);
        if (bytes_read > 0) {
            decoded_data = bwt_decode_block(buffer, bytes_read - INDEX_SIZE);
            fwrite(
                decoded_data, sizeof(*decoded_data), bytes_read - INDEX_SIZE,
                file_out);
            free(decoded_data);
        }
    } while (bytes_read == INDEX_SIZE + BLOCK_SIZE);
    // ------------------------------------------------------------------------
    fclose(file_out);
    fclose(file_in);
    return 0;
}
