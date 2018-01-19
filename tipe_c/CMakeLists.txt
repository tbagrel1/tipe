cmake_minimum_required(VERSION 3.9)
project(tipe_c C)

set(CMAKE_C_STANDARD 99)

add_executable(tipe_c main.c bwt_2.c bwt_2_launcher.c bwt_2.h bwt_2_launcher.h)
