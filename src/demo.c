#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "vendor/stb_image_write.h"

#include "krintc.h"
#include "point.h"

void write_png_image(const uint32_t *pixels, size_t pixel_width, size_t pixel_height, const char* filename)
{
    if (!stbi_write_png(filename, (int)pixel_width, (int)pixel_height, 4, pixels, sizeof(uint32_t) * pixel_width))
    {
        printf("Failed to write png to file!");
    }
}

size_t japan_example(void)
{
    u32 *pixels = NULL;
    uSize height = 800;
    uSize width = height * 3 / 2;
    uSize dotSize = height * 3 / 5;
    if (!krintc_alloc_data(&pixels, width, height)) return -1;

    krintc_fill(pixels, width, height, 0xFFFFFF);
    krintc_fill_circle(pixels, width, height, (i32)(width / 2), (i32)(height / 2), (i32)(dotSize / 2), 0x0000FF);

    write_png_image(pixels, width, height, "japan.png");

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

    write_png_image(pixels, width, height, "lines.png");

    krintc_free_data(&pixels);
    return 0;
}

#define OK(value) ((value) != (size_t)(-1))
#define LOG_NO_RETURN(id, value) do { printf("Exit(%d): %s\n", id, value); return id; } while(0)
#define LOG_RETURN(value) LOG_NO_RETURN(0, value)

int main(void)
{
    if (!OK(japan_example())) LOG_NO_RETURN(-1, "Failed japan example!");
    if (!OK(line_example())) LOG_NO_RETURN(-1, "Failed line example!");

    printf("Examples successful!");
    return 0;
}