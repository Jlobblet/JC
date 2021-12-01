cmake_minimum_required(VERSION 3.16)
project(JC C)

set(CMAKE_C_STANDARD 11)

add_library(JC jint.c jint.h jio.c jio.h jtime.c jtime.h)
