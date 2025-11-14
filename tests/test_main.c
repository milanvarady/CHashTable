#include "munit.h"

// Declare external test arrays
extern MunitTest table_create_destroy[];
extern MunitTest table_insert_get[];
extern MunitTest table_delete[];
extern MunitTest table_resize[];
extern MunitTest table_foreach[];
extern MunitTest table_persistence[];
extern MunitTest utils[];
extern MunitTest argument_parser[];

// Create sub-suites
static MunitSuite sub_suites[] = {
    { "/create", table_create_destroy, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { "/insert_get", table_insert_get, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { "/delete", table_delete, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { "/resize", table_resize, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { "/foreach", table_foreach, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { "/persistence", table_persistence, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { "/utils", utils, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { "/parser", argument_parser, nullptr, 1, MUNIT_SUITE_OPTION_NONE },
    { nullptr, nullptr, nullptr, 0, MUNIT_SUITE_OPTION_NONE }
};

// Main suite
static const MunitSuite suite = {
    "/ht",
    nullptr,           // No tests at root level
    sub_suites,     // Use sub-suites instead
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, nullptr, argc, argv);
}