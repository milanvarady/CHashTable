#include "../munit.h"
#include "../test_utils.h"

static MunitResult
test_table_eq(const MunitParameter params[], void *fixture) {
    HashTable *table_arr[2];

    for (int i = 0; i < 2; i++) {
        table_arr[i] = hash_table_create();
        hash_table_insert(table_arr[i], 1, 2);
        hash_table_insert(table_arr[i], 2, 3);
    }

    munit_assert_true(hash_table_equal(table_arr[0], table_arr[1]));
    hash_table_insert(table_arr[0], 5, 6);
    munit_assert_false(hash_table_equal(table_arr[0], table_arr[1]));

    for (int i = 0; i < 2; i++) {
        hash_table_destroy(table_arr[i]);
    }

    return MUNIT_OK;
}

MunitTest table_equal[] = {
    {"/equal", test_table_eq, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr},
    {nullptr, nullptr, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr}
};
