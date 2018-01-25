cmake_minimum_required(VERSION 3.9)
project(tipe_c C)

set(CMAKE_C_STANDARD 99)

add_executable(
    main_exe main.c
)

add_executable(
    bwt_exe bwt/main_bwt.c bwt/bwt_2.c bwt/bwt_2.h bwt/bwt_2_launcher.c bwt/bwt_2_launcher.h
)

add_executable(
    suffix_array_exe suffix_array/main_suffix_array.c suffix_array/suffix_array.c suffix_array/suffix_array.h suffix_array/clist.c suffix_array/clist.h
)
