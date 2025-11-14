#include "../munit.h"
#include "../test_utils.h"

static MunitResult
test_table_copy(const MunitParameter params[], void* fixture) {
    HashTable *table = (HashTable*)fixture;

    for (int i = 0; i < 10; i++) {
        hash_table_insert(table, i, i + 1);
    }

    HashTable *copy = hash_table_copy(table);

    munit_assert_not_null(copy);
    munit_assert_true(hash_table_equal(table, copy));

    // Check if they are distinct
    hash_table_insert(copy, 64, 65);
    munit_assert_false(hash_table_equal(table, copy));

    // Check nullptr
    hash_table_destroy(copy);
    copy = nullptr;

    munit_assert_null(hash_table_copy(copy));

    return MUNIT_OK;
}

MunitTest table_copy[] = {
    { "/copy", test_table_copy, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, nullptr },
    { nullptr, nullptr, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr }
};