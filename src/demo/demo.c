#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "shared/strutil.h"
#include "shared/files.h"
#include "args.h"
#include "tests.h"
#include "shared/mem.h"

#include "vendor/stb_image.h"

#include "examples.h"
#include "tests/record.h"

#define OK(value) ((value) != (size_t)(-1))
#define LOG_NO_RETURN(id, value) do { printf("Exit(%d): %s\n", id, value); return id; } while(0)
#define LOG_RETURN(value) LOG_NO_RETURN(0, value)

bool run_test(const char *result, const char *expected);

char *get_normal_name(const char *name)
{
	const char *suffix = "_expected";
	size_t size = strlen(name) - strlen(suffix);
	char *filename = mem_alloc(size + 1);
	memset(filename, 0, sizeof(char) * (size + 1));
	memcpy(filename, name, size - 4);
	memcpy(filename + (size - 4), ".png", 4);
	return filename;
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

    if (!OK(japan_example())) LOG_NO_RETURN(-1, "Failed japan example!");
    if (!OK(line_example())) LOG_NO_RETURN(-1, "Failed line example!");
	if (!OK(triangle_example())) LOG_NO_RETURN(-1, "Failed triangle example!");
	if (!OK(blend_example())) LOG_NO_RETURN(-1, "Failed blend example!");
	if (!OK(text_example())) LOG_NO_RETURN(-1, "Failed text example!");

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
    else if (test)
    {
        printf("Running tests...\n");
        int fails = 0;
        int success = 0;
        int tests = 0;
        for (size_t file_index = 0; file_index < test_count; file_index++)
        {
            const char *filename = tests_get_file(file_index);
			char *nfilename = get_normal_name(filename);
            printf("Running test for '%s' against '%s'\n", nfilename, filename);
            char *result_file = file_path_concat(".", nfilename);
            char *test_file = file_path_concat(args->test_dir, filename);

            if (!run_test(result_file, test_file))
            {
                printf("Test failed for '%s'\n", filename);
                fails++;
            }
            else
            {
                success++;
            }

			mem_free(nfilename);
            mem_free(result_file);
            mem_free(test_file);

            tests++;
        }

        printf("\nAll tests completed.\n");
        if (fails) printf("Failed tests: %d/%d (%d%%)\n", fails, tests, fails * 100 / tests);
        if (success) printf("Successful tests: %d/%d (%d%%)\n", success, tests, success * 100 / tests);
        printf("Test outcome: %s!\n", fails ? "FAILED" : "SUCCESS");
    }

    if (!test)
    {
        printf("Examples successful!\n");
    }

    tests_free();
    args_free();

    mem_validate(MEMV_ANY);

    return 0;
}

bool load_png(const char *filename, uint32_t **pixels, size_t *pwidth, size_t *pheight)
{
    if (pixels == NULL || pwidth == NULL || pheight == NULL) return false;

    int width, height, channels;
    stbi_uc* data = stbi_load(filename, &width, &height, &channels, 4);
    *pwidth = width;
    *pheight = height;

    if (channels != 4 && channels != 3)
    {
        printf("Unsupported image channel count: %d\n", channels);
        stbi_image_free(data);
        return false;
    }

    uint32_t *result = (uint32_t*)mem_alloc(width * height * sizeof(uint32_t));

    uint32_t *data32 = (uint32_t*)data;
    size_t size = (size_t)(width * height);
    for (size_t i = 0; i < size; i++)
    {
        uint32_t color = data32[i];
        if (channels == 3)
        {
            color |= 0xFF000000;
        }

        result[i] = color;
    }

    stbi_image_free(data);

    *pixels = result;

    return true;
}

bool run_test(const char *result, const char *expected)
{
    if (!file_exists(result) || !file_exists(expected))
    {
        return false;
    }

    uint32_t *result_pixels = NULL;
    uint32_t *expected_pixels = NULL;

    size_t result_width, result_height, expected_width, expected_height;

    if (!load_png(result, &result_pixels, &result_width, &result_height))
    {
        printf("Failed to load input png!\n");
        return false;
    }

    if (!load_png(expected, &expected_pixels, &expected_width, &expected_height))
    {
        mem_free(result_pixels);
        printf("Failed to load expected png!\n");
        return false;
    }

    bool success = true;

    if (result_width != expected_width)
    {
        printf("Image width does not match!\n");
        success = false;
    }

    if (result_height != expected_height)
    {
        printf("Image height does not match!\n");
        success = false;
    }

    if (!success)
    {
        mem_free(result_pixels);
        mem_free(expected_pixels);
        return false;
    }

    for (size_t i = 0; i < result_width * result_height; i++)
    {
        if (result_pixels[i] != expected_pixels[i])
        {
            success = false;
            break;
        }
    }

    mem_free(result_pixels);
    mem_free(expected_pixels);
    
    return success;
}
