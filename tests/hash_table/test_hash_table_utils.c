#include "../munit.h"
#include "../test_utils.h"

static MunitResult
test_hash_function_positive(const MunitParameter params[], void *fixture) {
    munit_assert_size(hash_function(0, 10), ==, 0);
    munit_assert_size(hash_function(10, 10), ==, 0);
    munit_assert_size(hash_function(2, 10), ==, 2);
    munit_assert_size(hash_function(12, 10), ==, 2);

    return MUNIT_OK;
}

static MunitResult
test_hash_function_negative(const MunitParameter params[], void *fixture) {
    munit_assert_size(hash_function(-1, 10), ==, 9);
    munit_assert_size(hash_function(-12, 10), ==, 8);
    munit_assert_size(hash_function(-10, 10), ==, 0);

    return MUNIT_OK;
}

static MunitResult
test_is_prime(const MunitParameter params[], void *fixture) {
    const size_t primes[5] = {2, 3, 53, 107, 7919};
    const size_t composites[5] = {0, 1, 4, 105, 108};

    for (int i = 0; i < 5; i++) {
        munit_assert_true(is_prime(primes[i]));
        munit_assert_false(is_prime(composites[i]));
    }

    return MUNIT_OK;
}

static MunitResult
test_next_prime(const MunitParameter params[], void *fixture) {
    munit_assert_size(next_prime(2), ==, 3);
    munit_assert_size(next_prime(3), ==, 5);
    munit_assert_size(next_prime(53), ==, 59);
    munit_assert_size(next_prime(7907), ==, 7919);

    return MUNIT_OK;
}

MunitTest utils[] = {
    {"/hash_function_positive", test_hash_function_positive, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr},
    {"/hash_function_negative", test_hash_function_negative, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr},
    {"/is_prime", test_is_prime, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr},
    {"/next_prime", test_next_prime, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr},
    {nullptr, nullptr, nullptr, nullptr, MUNIT_TEST_OPTION_NONE, nullptr}
};
