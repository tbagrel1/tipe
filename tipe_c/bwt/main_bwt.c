

#include "bwt_2_launcher.h"

int main(int argc, char **argv) {
    int return_code = 0;
    str in_file_a = "/home/thomas/Bureau/calgary.tar";
    str out_file = "/home/thomas/Bureau/calgary.bwt";
    return_code += 10 * bwt_code(in_file_a, out_file);
    /*
    str in_file_b = "/home/thomas/Bureau/calgary.dbwt";
    return_code += bwt_decode(out_file, in_file_b);
    */
    return return_code;
}
