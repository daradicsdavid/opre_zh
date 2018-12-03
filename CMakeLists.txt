cmake_minimum_required(VERSION 3.12)
project(opre_zh C)

set(CMAKE_C_STANDARD 99)

add_executable(opre_zh main.c print/print.c print/print.h array/array.h array/array.c struct/example_struct.h struct/struct.c struct/struct.h pipe/pipe.c pipe/pipe.h signal/signal.c signal/signal.h)