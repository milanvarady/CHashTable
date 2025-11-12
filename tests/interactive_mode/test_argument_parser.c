#include <stdio.h>
#include <string.h>

#include "../munit.h"
#include "../../src/interactive_mode/argument_parser_internal.h"

# define INVALID_INPUTS_COUNT 10
constexpr char invalid_inputs[INVALID_INPUTS_COUNT][32] = {
    "add 1 a",
    "add a 1",
    "add a a",
    "add",
    "get a",
    "get",
    "del a",
    "del",
    "makosretes",
    ""
};

#define VALID_INPUTS_COUNT 3
constexpr char valid_inputs[VALID_INPUTS_COUNT][32] = {
    "add 1 2",
    "get 1",
    "del 1"
};

static InteractiveModeInput mock_input(const char *input) {
    FILE* dev_null = fopen("/dev/null", "w");
    munit_assert_not_null(dev_null);

    FILE* test_stream = tmpfile();
    munit_assert_not_null(test_stream);

    char mock_input[32];
    strcpy(mock_input, input);
    strcat(mock_input, "\n");

    fputs(mock_input, test_stream);
    rewind(test_stream);

    InteractiveModeInput result = get_input(test_stream, dev_null);

    fclose(test_stream);

    return result;
}

static MunitResult
test_valid_commands(const MunitParameter params[], void* fixture) {
    for (int i = 0; i < VALID_INPUTS_COUNT; i++) {
        char input[32];
        sprintf(input, "%s %d %d\n", command_table[i].cmd_name, 1, 2);
        InteractiveModeInput result = mock_input(input);

        munit_assert_int(result.cmd, !=, CMD_INVALID);
        munit_assert_int(result.cmd, ==, command_table[i].result_value);
    }

    return MUNIT_OK;
}

static MunitResult
test_valid_arguments(const MunitParameter params[], void* fixture) {
    // Test key argument
    for (int i = 0; i < VALID_INPUTS_COUNT; i++) {
        InteractiveModeInput result = mock_input(valid_inputs[i]);

        munit_assert_int(result.cmd, !=, CMD_INVALID);
        munit_assert_int(result.arg_key, ==, 1);
    }

    // Test value argument for add
    InteractiveModeInput result = mock_input("add 1 2");
    munit_assert_int(result.arg_key, ==, 1);
    munit_assert_int(result.arg_value, ==, 2);

    return MUNIT_OK;
}

static MunitResult
test_invalid_arguments(const MunitParameter params[], void* fixture) {
    for (int i = 0; i < INVALID_INPUTS_COUNT; i++) {
        InteractiveModeInput result = mock_input(invalid_inputs[i]);

        munit_assert_int(result.cmd, ==, CMD_INVALID);
    }

    return MUNIT_OK;
}

MunitTest argument_parser[] = {
    { "/commands_valid", test_valid_commands, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/arguments_valid", test_valid_arguments, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { "/arguments_invalid", test_invalid_arguments, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};