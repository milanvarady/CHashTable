#include "munit.h"
#include "../src/hash_table.h"

static void*
hash_table_setup(const MunitParameter params[], void* user_data) {
    HashTable* table = hash_table_create();
    return table;
}

static void
hash_table_teardown(void* fixture) {
    HashTable* table = (HashTable*)fixture;
    hash_table_destroy(table);
}

static MunitResult
test_create_hash_table(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;
    hash_table_insert(table, 1, 10);

    munit_assert_not_null(table);
    munit_assert_size(table->size, ==, HT_INITIAL_SIZE);
    munit_assert_size(table->count, ==, 1);

    return MUNIT_OK;
}

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

static MunitResult
test_resizing(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    for (int i = 0; i < 20; i++) {
        hash_table_insert(table, i, i * 10);
    }

    munit_assert_size(table->size, ==, HT_INITIAL_SIZE);
    munit_assert_size(table->count, ==, 20);

    for (int i = 20; i < 40; i++) {
        hash_table_insert(table, i, i * 10);
    }

    munit_assert_size(table->size * 2, >, HT_INITIAL_SIZE);
    munit_assert_size(table->count, ==, 40);

    return MUNIT_OK;
}

static MunitResult
test_data_persists_after_resize(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    // Insert enough to trigger resize
    for (int i = 0; i < 50; i++) {
        hash_table_insert(table, i, i * 10);
    }

    // Verify old data is still there
    for (int i = 0; i < 50; i++) {
        munit_assert_int(hash_table_get(table, i, -1), ==, i * 10);
    }

    return MUNIT_OK;
}

static MunitResult
test_delete(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    // Test with multiple values
    for (int i = 0; i < 5; i++) {
        hash_table_insert(table, (int)HT_INITIAL_SIZE * i, i * 10);
    }

    for (int i = 0; i < 5; i++) {
        munit_assert_int(hash_table_get(table,(int)HT_INITIAL_SIZE * i, -1), ==, i * 10);
    }

    // Delete from head
    munit_assert_size(table->count, ==, 5);
    munit_assert_true(hash_table_delete(table, 0));
    munit_assert_false(hash_table_delete(table, 0));
    munit_assert_size(table->count, ==, 4);

    // Delete from middle
    munit_assert_true(hash_table_delete(table, (int)HT_INITIAL_SIZE * 2));
    munit_assert_false(hash_table_delete(table, (int)HT_INITIAL_SIZE * 2));
    munit_assert_size(table->count, ==, 3);

    // Delete from tail
    munit_assert_true(hash_table_delete(table, (int)HT_INITIAL_SIZE * 4));
    munit_assert_false(hash_table_delete(table, (int)HT_INITIAL_SIZE * 4));
    munit_assert_size(table->count, ==, 2);

    return MUNIT_OK;
}

static MunitResult
test_delete_all(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    for (int i = 0; i < 10; i++) {
        hash_table_insert(table, i, i * 10);
    }

    munit_assert_size(table->count, ==, 10);

    for (int i = 0; i < 10; i++) {
        munit_assert_true(hash_table_delete(table, i));
    }

    munit_assert_size(table->count, ==, 0);

    // Verify all are actually gone
    for (int i = 0; i < 10; i++) {
        munit_assert_int(hash_table_get(table, i, -1), ==, -1);
    }

    return MUNIT_OK;
}

void assert_foreach(int key, int value, void* run_count) {
    munit_assert_int(value, ==, key * 10);
    *(int*)run_count += 1;
}

static MunitResult
test_foreach(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    for (int i = 1; i < 5; i++) {
        hash_table_insert(table, i, i * 10);
    }

    for (int i = 0; i < 5; i++) {
        hash_table_insert(table, (int)HT_INITIAL_SIZE * i, (int)HT_INITIAL_SIZE * i * 10);
    }

    int run_count = 0;
    hash_table_foreach(table, assert_foreach, &run_count);

    munit_assert_int(run_count, ==, 9);

    return MUNIT_OK;
}

static MunitTest tests[] = {
    { "/create_table", test_create_hash_table, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/insert_get", test_insert_and_get, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/zero_key", test_zero_key, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/negative_keys", test_negative_keys, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/update_value", test_update_value, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/multiple_updates", test_multiple_updates, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/get_default_empty", test_get_default_empty_table, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/collision_handling", test_collision_handling, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/test_resizing", test_resizing, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/data_persists_after_resize", test_data_persists_after_resize, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/test_delete", test_delete, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/delete_all", test_delete_all, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/test_foreach", test_foreach, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    "/hash_table", tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}