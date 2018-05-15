cmake_minimum_required(VERSION 3.9)
project(tipe_c C)

set(CMAKE_C_STANDARD 99)

add_executable(
    main_exe main.c)

add_executable(
    bwt_exe bwt/main_bwt.c bwt/bwt_2.c bwt/bwt_2.h bwt/bwt_2_launcher.c bwt/bwt_2_launcher.h
)

add_executable(
    suffix_array_exe suffix_array/main_suffix_array.c suffix_array/suffix_array.c suffix_array/suffix_array.h suffix_array/clist.c suffix_array/clist.h suffix_array/sais.c suffix_array/sais.h
)

add_executable(
    bitwise_ppm_exe bitwise_ppm/main_bitwise_ppm.c bitwise_ppm/bitwise_ppm.c bitwise_ppm/bitwise_ppm.h
    bitwise_ppm/arithmetic_coder.c bitwise_ppm/arithmetic_coder.h
    bitwise_ppm/defs.h bitwise_ppm/defs.c bitwise_ppm/cvector_core.h
    bitwise_ppm/cvector_interface.h)
target_link_libraries(bitwise_ppm_exe m)
