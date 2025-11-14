cmake_minimum_required(VERSION 4.0)
project(CHashTable C)

set(CMAKE_C_STANDARD 23)
#set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

# Main executable
add_executable(CHashTable
        src/hash_table/hash_table_core.c
        src/hash_table/hash_table_io.c
        src/hash_table/hash_table_resize.c
        src/hash_table/hash_table_utils.c
        src/fprintf_color/fprintf_color.c
        src/interactive_mode/argument_parser.c
        src/main.c)

# Test executable
add_executable(CHashTable_tests
        tests/munit.c
        src/hash_table/hash_table_core.c
        src/hash_table/hash_table_io.c
        src/hash_table/hash_table_resize.c
        src/hash_table/hash_table_utils.c
        src/fprintf_color/fprintf_color.c
        src/interactive_mode/argument_parser.c
        tests/hash_table/test_hash_table_create_destroy.c
        tests/hash_table/test_hash_table_insert_get.c
        tests/hash_table/test_hash_table_resize.c
        tests/hash_table/test_hash_table_delete.c
        tests/hash_table/test_hash_table_foreach.c
        tests/hash_table/test_hash_table_persistence.c
        tests/hash_table/test_hash_table_equal.c
        tests/hash_table/test_hash_table_copy.c
        tests/hash_table/test_hash_table_utils.c
        tests/interactive_mode/test_argument_parser.c
        tests/test_main.c)
