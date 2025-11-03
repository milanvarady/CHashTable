cmake_minimum_required(VERSION 4.0)
project(CHashTable C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

# Main executable
add_executable(CHashTable
        src/hash_table.c
        src/hash_table.h
        src/main.c)

# Test executable
add_executable(CHashTable_tests
        src/hash_table.c
        src/hash_table.h
        src/hash_table_internal.h
        tests/munit.c
        tests/test_hash_table.c)

target_include_directories(CHashTable_tests PRIVATE tests)
target_compile_definitions(CHashTable_tests PRIVATE TESTING)