cmake_minimum_required(VERSION 4.0)
project(CHashTable C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

add_executable(CHashTable
        src/hash_table.c
        src/hash_table.h
        src/main.c)
