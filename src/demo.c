#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "krintc.h"
#include "point.h"
#include "strutil.h"
#include "files.h"
#include "args.h"
#include "tests.h"
#include "mem.h"

#include "vendor/stb_image.h"

size_t japan_example(void)
{
    u32 *pixels = NULL;
    uSize height = 800;
    uSize width = height * 3 / 2;
    uSize dotSize = height * 3 / 5;
    if (!krintc_alloc_data(&pixels, width, height)) return -1;

    krintc_fill(pixels, width, height, 0xFFFFFFFF);
    krintc_fill_circle(pixels, width, height, (i32)(width / 2), (i32)(height / 2), (i32)(dotSize / 2), 0xFF0000FF);

    if (!krintc_save_disk_image(pixels, width, height, "japan.png"))
    {
        printf("Failed to save japan.png!\n");
    }

    krintc_free_data(&pixels);
    return 0;
}

point_t get_point(u32 index, u32 size, u32 total_index)
{
    f32 angleBase = 3.141592f / ((f32)total_index / 2.0f);
    f32 angle = angleBase * index;
    f32 x = cosf(angle);
    f32 y = sinf(angle);

    return point_create((i32)(x * size), (i32)(y * size));
}

size_t line_example(void)
{
    u32 *pixels = NULL;
    uSize width = 200;
    uSize height = 200;
    if (!krintc_alloc_data(&pixels, width, height)) return -1;

    krintc_fill(pixels, width, height, 0xFF000000);
    
    point_t basePosPoint = point_create(100, 100);
    for (u32 index = 0; index < 18; index++)
    {
        point_t current = get_point(index, 80, 18);
        point_t next = get_point((index + 1) % 18, 80, 18);
        point_add_self_point(&current, basePosPoint);
        point_add_self_point(&next, basePosPoint);

        krintc_line(pixels, width, height, 100, 100, current.x, current.y, index % 2 ? 0xFFFF0000 : 0xFF00FFFF);
        krintc_line(pixels, width, height, current.x, current.y, next.x, next.y, 0xFFFFFF00);
    }    

    if (!krintc_save_disk_image(pixels, width, height, "lines.png"))
    {
        printf("Failed to save lines.png!\n");
    }

    krintc_free_data(&pixels);
    return 0;
}

size_t triangle_example(void)
{
	u32 *pixels = NULL;
	uSize width = 800;
	uSize height = 800;
	if (!krintc_alloc_data(&pixels, width, height)) return -1;

	krintc_fill(pixels, width, height, 0xFF000000);

	uSize w2 = width / 2;
	uSize h2 = height / 2;

	point_t basePosPoint = point_create(w2, h2);
	for (u32 index = 0; index < 6; index++)
	{
		// rolled out version of (w2 + h2) / 2 * 4 / 5
		u32 radius = (w2 + h2) * 2 / 5;

		point_t current = get_point(index, radius, 6);
		point_t next = get_point((index + 1) % 6, radius, 6);
		point_add_self_point(&current, basePosPoint);
		point_add_self_point(&next, basePosPoint);

		krintc_fill_triangle(pixels, width, height, w2, h2, current.x, current.y, next.x, next.y, index % 2 ? 0xFF00AAAA : 0xFF00FFFF);
	}
	
	if (!krintc_save_disk_image(pixels, width, height, "triangle.png"))
	{
		printf("Failed to save triangle.png!\n");
	}

	krintc_free_data(&pixels);
	return 0;
}

size_t blend_example(void)
{
	u32 *pixels = NULL;
	uSize width = 400;
	uSize height = 400;
	if (!krintc_alloc_data(&pixels, width, height)) return -1;

	krintc_fill(pixels, width, height, 0xFF000000);

	krintc_fill_rect(pixels, width, height, 0, 0, 300, 300, 0xFF00FF00);
	krintc_fill_rect(pixels, width, height, 100, 100, 400, 400, 0xAA0000FF);
	krintc_fill_circle(pixels, width, height, 100, 100, 50, 0x77FF0000);
	krintc_fill_triangle(pixels, width, height, 50, 100, 0, 140, 120, 350, 0xCCFF00FF);

	if (!krintc_save_disk_image(pixels, width, height, "alphablend.png"))
	{
		printf("Failed to save alphablend.png!\n");
	}

	krintc_free_data(&pixels);
	return 0;
}

#define OK(value) ((value) != (size_t)(-1))
#define LOG_NO_RETURN(id, value) do { printf("Exit(%d): %s\n", id, value); return id; } while(0)
#define LOG_RETURN(value) LOG_NO_RETURN(0, value)

bool file_copy_into(const char *srcdir, const char *dstdir, const char *filename, const char *filename_dst)
{
	if (filename_dst == NULL)
		filename_dst = filename;

    char *srcfile = file_path_concat(srcdir, filename);
    char *dstfile = file_path_concat(dstdir, filename_dst);

    if (!file_exists(srcfile))
    {
        mem_free(srcfile);
        mem_free(dstfile);

        return false;
    }

    bool result = file_copy(srcfile, dstfile);

    mem_free(srcfile);
    mem_free(dstfile);

    return result;
}

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

    if (record)
    {
		const char *suffix = "_expected";
		ptrlist_t *image_files = file_list_dir(".", ".png");
		size_t image_count = file_list_size(image_files);
        for (size_t file_index = 0; file_index < image_count; file_index++)
        {
			const char *filename = file_list_get(image_files, file_index);
			size_t len = strlen(filename) + strlen(suffix) + 1;
            char *filename_expected = mem_alloc(len);
			memset(filename_expected, 0, sizeof(char) * len);
			strncpy(filename_expected, filename, strlen(filename) - 4);
			strcat(filename_expected, suffix);
			strcat(filename_expected, ".png");
			if (!file_copy_into(".", args->test_dir, filename, filename_expected))
            {
                char* dstpath = file_path_concat(args->test_dir, filename_expected);
                printf("Failed to copy file './%s' into '%s'!\n", filename, dstpath);
                mem_free(dstpath);
            }
			mem_free(filename_expected);
        }

		file_list_dir_free(&image_files);
    }
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
