#include "test.h"

#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "../tests.h"
#include "shared/mem.h"

#include "vendor/stb_image.h"
#include "shared/files.h"

static bool run_test(const char *result, const char *expected);

static char *get_normal_name(const char *name)
{
	const char *suffix = "_expected";
	size_t size = strlen(name) - strlen(suffix);
	char *filename = mem_alloc(size + 1);
	memset(filename, 0, sizeof(char) * (size + 1));
	memcpy(filename, name, size - 4);
	memcpy(filename + (size - 4), ".png", 4);
	return filename;
}

void tests_test(size_t test_count, const args_t *args)
{
	printf("Running tests...\n");

	int32_t fails = 0;
	int32_t success = 0;
	int32_t tests = 0;

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
		else success++;

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

static bool load_png(const char *filename, uint32_t **pixels, size_t *pwidth, size_t *pheight)
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

static bool run_test(const char *result, const char *expected)
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

