#include "munit.h"
#include "utils.h"

static MunitResult
test_delete(const MunitParameter params[], void* fixture) {
    HashTable* table = (HashTable*)fixture;

    // Test with multiple values
    for (int i = 0; i < 5; i++) {
        hash_table_insert(table, (int)HT_INITIAL_SIZE * i, i * 10);
    }

    for (int i = 0; i < 5; i++) {
        const Entry *entry = hash_table_get(table,(int)HT_INITIAL_SIZE * i);
        munit_assert_not_null(entry);
        munit_assert_int(entry->value, ==, i * 10);
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
        munit_assert_null(hash_table_get(table, i));
    }

    return MUNIT_OK;
}

MunitTest table_delete[] = {
    { "/base", test_delete, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/all", test_delete_all, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};