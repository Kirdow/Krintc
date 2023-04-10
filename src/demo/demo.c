#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "shared/strutil.h"
#include "shared/files.h"
#include "args.h"
#include "tests.h"
#include "shared/mem.h"


#include "examples.h"
#include "tests/record.h"
#include "tests/test.h"

#define OK(value) ((value) != (size_t)(-1))
#define LOG_NO_RETURN(id, value) do { printf("Exit(%d): %s\n", id, value); return id; } while(0)
#define LOG_RETURN(value) LOG_NO_RETURN(0, value)

size_t run_examples(void)
{
    if (!OK(japan_example())) LOG_NO_RETURN(-1, "Failed japan example!");
    if (!OK(line_example())) LOG_NO_RETURN(-1, "Failed line example!");
	if (!OK(triangle_example())) LOG_NO_RETURN(-1, "Failed triangle example!");
	if (!OK(blend_example())) LOG_NO_RETURN(-1, "Failed blend example!");
	if (!OK(text_example())) LOG_NO_RETURN(-1, "Failed text example!");

	return 0;
}

int main(int argc, const char *argv[])
{
    args_load(argc, argv);
    tests_load();
    const args_t *args = args_get();

    size_t test_count = tests_get_file_count();

    for (size_t file_index = 0; file_index < test_count; file_index++)
    {
        file_delete(tests_get_file(file_index));
    }

	if (!OK(run_examples())) return -1;

    bool record = args->record;
    bool test = args->test;
    if (args->test_dir)
    {
        if (!file_exists(args->test_dir))
        {
            printf("Directory not found: '%s'!\n", args->test_dir);
            record = false;
            test = false;
        }
    }
    else
    {
        record = false;
        test = false;
    }

    if (record) tests_record(args);
    else if (test) tests_test(test_count, args);

    if (!test) printf("Examples successful!\n");

    tests_free();
    args_free();

    mem_validate(MEMV_ANY);

    return 0;
}

