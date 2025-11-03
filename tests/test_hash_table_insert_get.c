#include "munit.h"
#include "utils.h"

static MunitResult
test_insert_and_get(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 1, 10);
    hash_table_insert(table, (int)HT_INITIAL_SIZE + 1, 540);

    munit_assert_int(hash_table_get(table, 1, -1), ==, 10);
    munit_assert_int(hash_table_get(table, 2, -1), ==, -1);
    munit_assert_int(hash_table_get(table, (int)HT_INITIAL_SIZE + 1, -1), ==, 540);

    return MUNIT_OK;
}

static MunitResult
test_zero_key(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 0, 999);
    munit_assert_int(hash_table_get(table, 0, -1), ==, 999);

    return MUNIT_OK;
}

static MunitResult
test_negative_keys(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, -5, 100);
    hash_table_insert(table, -10, 200);

    munit_assert_int(hash_table_get(table, -5, -1), ==, 100);
    munit_assert_int(hash_table_get(table, -10, -1), ==, 200);

    return MUNIT_OK;
}

static MunitResult
test_update_value(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    hash_table_insert(table, 1, 10);
    hash_table_insert(table, 1, 100);

    munit_assert_int(hash_table_get(table, 1, -1), ==, 100);
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

    munit_assert_int(hash_table_get(table, 5, -1), ==, 40);
    munit_assert_size(table->count, ==, 1);  // Still only 1 entry

    return MUNIT_OK;
}

// Test: get with default value on empty table
static MunitResult
test_get_default_empty_table(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    munit_assert_int(hash_table_get(table, 999, 42), ==, 42);
    munit_assert_int(hash_table_get(table, 0, -999), ==, -999);

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
        munit_assert_int(hash_table_get(table, key, -1), ==, i * 100);
    }

    return MUNIT_OK;
}

MunitTest table_insert_get[] = {
    { "/base", test_insert_and_get, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/zero_key", test_zero_key, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/negative_keys", test_negative_keys, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/update_value", test_update_value, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/multiple_updates", test_multiple_updates, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/get_default_empty", test_get_default_empty_table, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/collision_handling", test_collision_handling, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};