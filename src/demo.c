#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "krintc.h"
#include "point.h"
#include "strutil.h"
#include "files.h"

size_t japan_example(void)
{
    u32 *pixels = NULL;
    uSize height = 800;
    uSize width = height * 3 / 2;
    uSize dotSize = height * 3 / 5;
    if (!krintc_alloc_data(&pixels, width, height)) return -1;

    krintc_fill(pixels, width, height, 0xFFFFFF);
    krintc_fill_circle(pixels, width, height, (i32)(width / 2), (i32)(height / 2), (i32)(dotSize / 2), 0x0000FF);

    if (!krintc_save_disk_image(pixels, width, height, "japan.png"))
    {
        printf("Failed to save japan.png!\n");
    }

    krintc_free_data(&pixels);
    return 0;
}

point_t get_point(u32 index, u32 size)
{
    f32 angleBase = 3.141592f / 9.0f;
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

    krintc_fill(pixels, width, height, 0);
    
    point_t basePosPoint = point_create(100, 100);
    for (u32 index = 0; index < 18; index++)
    {
        point_t current = get_point(index, 80);
        point_t next = get_point((index + 1) % 18, 80);
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

#define OK(value) ((value) != (size_t)(-1))
#define LOG_NO_RETURN(id, value) do { printf("Exit(%d): %s\n", id, value); return id; } while(0)
#define LOG_RETURN(value) LOG_NO_RETURN(0, value)

bool file_copy_into(const char *srcdir, const char *dstdir, const char *filename)
{
    char *srcfile = file_path_concat(srcdir, filename);
    char *dstfile = file_path_concat(dstdir, filename);

    if (!file_exists(srcfile))
    {
        free(srcfile);
        free(dstfile);

        return false;
    }

    bool result = file_copy(srcfile, dstfile);

    free(srcfile);
    free(dstfile);

    return result;
}

int main(int argc, const char *argv[])
{
    #define NEXTARG(x) do { i += 1; if (i >= argc) break; x = argv[i]; } while (0)
    const char *test_dir = NULL;
    bool record = false;
    for (int i = 1; i < argc; i++)
    {
        const char* arg = NULL;
        if (str_equals(argv[i], "--testdir"))
        {
            NEXTARG(arg);
            test_dir = arg;
        }
        else if (str_equals(argv[i], "--record=true"))
        {
            record = true;
        }
        else if (str_equals(argv[i], "--record=false"))
        {
            record = false;
        }
    }

    #define FILE_COUNT 2
    const char* files[FILE_COUNT] = {
        "japan.png",
        "lines.png"
    };

    for (int file_index = 0; file_index < FILE_COUNT; file_index++)
    {
        file_delete(files[file_index]);
    }

    if (!OK(japan_example())) LOG_NO_RETURN(-1, "Failed japan example!");
    if (!OK(line_example())) LOG_NO_RETURN(-1, "Failed line example!");

    if (test_dir)
    {
        if (!file_exists(test_dir))
        {
            printf("Directory not found: '%s'!\n", test_dir);
            record = false;
        }
    }

    if (record)
    {
        for (int file_index = 0; file_index < FILE_COUNT; file_index++)
        {
            const char *filename = files[file_index];
            if (!file_copy_into(".", "./../tests", filename))
            {
                printf("Failed to copy file './%s' into './../tests/%s'!\n", filename, filename);
            }            
        }
    }

    printf("Examples successful!");
    return 0;
}