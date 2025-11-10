cmake_minimum_required(VERSION 4.0)
project(CHashTable C)

set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

# Main executable
add_executable(CHashTable
        src/hash_table_core.c
        src/hash_table_io.c
        src/hash_table_resize.c
        src/hash_table_utils.c
        src/main.c
        src/printf_color.c
        src/printf_color.h)

# Test executable
add_executable(CHashTable_tests
        tests/munit.c
        src/hash_table_core.c
        src/hash_table_io.c
        src/hash_table_resize.c
        src/hash_table_utils.c
        tests/test_hash_table_create.c
        tests/test_hash_table_insert_get.c
        tests/test_hash_table_resize.c
        tests/test_hash_table_delete.c
        tests/test_hash_table_foreach.c
        tests/test_hash_table_utils.c
        tests/test_main.c)
