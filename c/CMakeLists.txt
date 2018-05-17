cmake_minimum_required(VERSION 3.9)
project(tipe_c C)

set(CMAKE_C_STANDARD 99)

add_executable(
    bitwise_ppm_exe bitwise_ppm/main_bitwise_ppm.c bitwise_ppm/bitwise_ppm.c bitwise_ppm/bitwise_ppm.h
    bitwise_ppm/arithmetic_coder.c bitwise_ppm/arithmetic_coder.h
    bitwise_ppm/defs.h bitwise_ppm/cvector_core.h
    bitwise_ppm/cvector_interface.h bitwise_ppm/bitwise_ppm_handler.c bitwise_ppm/bitwise_ppm_handler.h bitwise_ppm/acode_t_vect.c bitwise_ppm/acode_t_vect.h)

target_link_libraries(bitwise_ppm_exe m)

add_executable(
    bitwise_ppm_flat_exe bitwise_ppm_flat/acode_t_vect.c
    bitwise_ppm_flat/acode_t_vect.h bitwise_ppm_flat/cvector_interface.h
    bitwise_ppm_flat/cvector_core.h
    bitwise_ppm_flat/arithmetic_coder_flat.c bitwise_ppm_flat/arithmetic_coder_flat.h bitwise_ppm_flat/bitwise_ppm_flat.c bitwise_ppm_flat/bitwise_ppm_flat.h bitwise_ppm_flat/bitwise_ppm_flat_handler.c bitwise_ppm_flat/bitwise_ppm_flat_handler.h bitwise_ppm_flat/main_bitwise_ppm_flat.c bitwise_ppm_flat/defs.h)

target_link_libraries(bitwise_ppm_flat_exe m)

add_executable(
    rle_exe rle/rle_handler.c rle/rle_handler.h rle/main_rle.c
    rle/cvector_core.h rle/cvector_interface.h rle/rle_code_t_vect.c rle/rle_code_t_vect.h)

target_link_libraries(rle_exe m)
