#include "../munit.h"
#include "../test_utils.h"

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
        munit_assert_int(hash_table_get(table, i)->value, ==, i * 10);
    }

    return MUNIT_OK;
}

MunitTest table_resize[] = {
    { "/resize", test_resizing, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { "/data_persists", test_data_persists_after_resize, hash_table_setup, hash_table_teardown, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};