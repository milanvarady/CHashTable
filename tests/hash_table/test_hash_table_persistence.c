#include <stdio.h>

#include "../munit.h"
#include "../test_utils.h"
#include "../../src/hash_table/hash_table.h"

static MunitResult
test_save_errors(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    munit_assert_false(hash_table_save(nullptr, "a.txt"));
    munit_assert_false(hash_table_save(table, ""));

    return MUNIT_OK;
}

static MunitResult
test_save_success(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 1, 2);
    hash_table_insert(table, 2, 3);

    const char filename[] = "test_save_success.txt";

    munit_assert_true(hash_table_save(table, filename));

    munit_assert_int(remove(filename), ==, 0);

    return MUNIT_OK;
}

static MunitResult
test_load_errors(const MunitParameter params[], void* fixture) {
    HashTable *table = nullptr;

    munit_assert_int(
        hash_table_load("nonexistent.txt", &table),
        ==,
        HT_LOAD_ERROR_FILE_OPEN
    );

    munit_assert_int(
        hash_table_load("../tests/fixtures/ht_emtpy.txt", &table),
        ==,
        HT_LOAD_ERROR_EMPTY
    );

    munit_assert_int(
        hash_table_load("../tests/fixtures/ht_invalid_header.txt", &table),
        ==,
        HT_LOAD_ERROR_INVALID_HEADER
    );

    munit_assert_int(
        hash_table_load("../tests/fixtures/ht_no_count.txt", &table),
        ==,
        HT_LOAD_ERROR_MISSING_COUNT
    );

    munit_assert_int(
        hash_table_load("../tests/fixtures/ht_malformed_count.txt", &table),
        ==,
        HT_LOAD_ERROR_MALFORMED_COUNT
    );

    munit_assert_int(
        hash_table_load("../tests/fixtures/ht_premature_eof.txt", &table),
        ==,
        HT_LOAD_ERROR_PREMATURE_EOF
    );

    munit_assert_null(table);

    return MUNIT_OK;
}

static MunitResult
test_load_success(const MunitParameter params[], void* fixture) {
    HashTable *table = nullptr;

    munit_assert_int(
        hash_table_load("../tests/fixtures/ht_valid.txt", &table),
        ==,
        HT_LOAD_OK
    );

    munit_assert_not_null(table);

    const Entry *e1 = hash_table_get(table, 1);
    const Entry *e2 = hash_table_get(table, 2);

    munit_assert_not_null(e1);
    munit_assert_not_null(e2);

    munit_assert_int(e1->value, ==, 2);
    munit_assert_int(e2->value, ==, 3);

    return MUNIT_OK;
}

MunitTest table_persistence[] = {
    { "/save_error", test_save_errors, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, nullptr },
    { "/save_success", test_save_success, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, nullptr },
    { "/load_error", test_load_errors, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr },
    { "/load_success", test_load_success, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr },
    { nullptr, nullptr, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr }
};