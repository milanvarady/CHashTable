#include "../munit.h"
#include "../test_utils.h"

void assert_foreach(int key, int value, void *run_count) {
    munit_assert_int(value, ==, key * 10);
    *(int *) run_count += 1;
}

static MunitResult
test_foreach(const MunitParameter params[], void *fixture) {
    HashTable *table = (HashTable *) fixture;

    for (int i = 1; i < 5; i++) {
        hash_table_insert(table, i, i * 10);
    }

    for (int i = 0; i < 5; i++) {
        hash_table_insert(table, (int) HT_INITIAL_SIZE * i, (int) HT_INITIAL_SIZE * i * 10);
    }

    int run_count = 0;
    hash_table_foreach(table, assert_foreach, &run_count);

    munit_assert_int(run_count, ==, 9);

    return MUNIT_OK;
}

MunitTest table_foreach[] = {
    {"/foreach", test_foreach, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, nullptr},
    {nullptr, nullptr, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr}
};
