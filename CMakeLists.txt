cmake_minimum_required(VERSION 3.12)
project(opre_zh C)

set(CMAKE_C_STANDARD 99)

add_executable(opre_zh main.c print/print.c print/print.h array/array.h array/array.c struct/example_struct.h struct/struct.c struct/struct.h pipe/pipe.c pipe/pipe.h signal/zh_example/zh_example_signal.h read/read.c read/read.h message_queue/simple/example.c message_queue/simple/simple_with_fork.c shared_memory/simple/simple.c semaphore/simple/simple.c message_queue/zh/zh_example.c message_queue/zh/zh_example_signal.h signal/zh_example/zh_example_signal.c shared_memory/zh/zh_example_shared_memory.c shared_memory/zh/zh_example_shared_memory.h)