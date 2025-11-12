#include "../munit.h"
#include "../test_utils.h"

static MunitResult
test_insert_and_get(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 1, 10);
    hash_table_insert(table, (int)HT_INITIAL_SIZE + 1, 540);

    munit_assert_int(hash_table_get(table, 1)->value, ==, 10);
    munit_assert_null(hash_table_get(table, 2));
    munit_assert_int(hash_table_get(table, (int)HT_INITIAL_SIZE + 1)->value, ==, 540);

    return MUNIT_OK;
}

static MunitResult
test_zero_key(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 0, 999);
    munit_assert_int(hash_table_get(table, 0)->value, ==, 999);

    return MUNIT_OK;
}

static MunitResult
test_negative_keys(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, -5, 100);
    hash_table_insert(table, -10, 200);

    munit_assert_int(hash_table_get(table, -5)->value, ==, 100);
    munit_assert_int(hash_table_get(table, -10)->value, ==, 200);

    return MUNIT_OK;
}

static MunitResult
test_update_value(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 1, 10);
    hash_table_insert(table, 1, 100);

    munit_assert_int(hash_table_get(table, 1)->value, ==, 100);
    munit_assert_size(table->count, ==, 1);

    return MUNIT_OK;
}

static MunitResult
test_multiple_updates(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 5, 10);
    hash_table_insert(table, 5, 20);
    hash_table_insert(table, 5, 30);
    hash_table_insert(table, 5, 40);

    munit_assert_int(hash_table_get(table, 5)->value, ==, 40);
    munit_assert_size(table->count, ==, 1);  // Still only 1 entry

    return MUNIT_OK;
}

static MunitResult
test_collision_handling(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    // Insert keys that will collide (multiple of table size)
    for (int i = 0; i < 5; i++) {
        int key = (int)HT_INITIAL_SIZE * i;
        hash_table_insert(table, key, i * 100);
    }

    // Verify all values are retrievable
    for (int i = 0; i < 5; i++) {
        int key = (int)HT_INITIAL_SIZE * i;
        munit_assert_int(hash_table_get(table, key)->value, ==, i * 100);
    }

    return MUNIT_OK;
}

MunitTest table_insert_get[] = {
    { "/base", test_insert_and_get, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/zero_key", test_zero_key, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/negative_keys", test_negative_keys, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/update_value", test_update_value, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/multiple_updates", test_multiple_updates, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/collision_handling", test_collision_handling, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};