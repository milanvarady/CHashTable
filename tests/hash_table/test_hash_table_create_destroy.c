#include "../munit.h"
#include "../test_utils.h"

static MunitResult
test_create(const MunitParameter params[], void *fixture) {
    HashTable *table = (HashTable *) fixture;
    hash_table_insert(table, 1, 10);

    munit_assert_not_null(table);
    munit_assert_size(table->size, ==, HT_INITIAL_SIZE);
    munit_assert_size(table->count, ==, 1);

    return MUNIT_OK;
}

static MunitResult
test_destroy(const MunitParameter params[], void *fixture) {
    HashTable *table = hash_table_create();

    munit_assert_true(hash_table_destroy(table));
    munit_assert_false(hash_table_destroy(nullptr));

    return MUNIT_OK;
}

MunitTest table_create_destroy[] = {
    {"/create", test_create, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, nullptr},
    {"/destroy", test_destroy, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr},
    {nullptr, nullptr, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr}
};
