#include "munit.h"

// Declare external test arrays
extern MunitTest table_create[];
extern MunitTest table_insert_get[];
extern MunitTest table_delete[];
extern MunitTest table_resize[];
extern MunitTest table_foreach[];
extern MunitTest utils[];

// Create sub-suites
static MunitSuite sub_suites[] = {
    { "/create", table_create, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/insert_get", table_insert_get, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/delete", table_delete, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/resize", table_resize, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/foreach", table_foreach, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { "/utils", utils, NULL, 1, MUNIT_SUITE_OPTION_NONE },
    { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE }
};

// Main suite
static const MunitSuite suite = {
    "/ht",
    NULL,           // No tests at root level
    sub_suites,     // Use sub-suites instead
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}