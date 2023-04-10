#include "examples.h"

#include <math.h>

#include "krintc/krintc.h"
#include "krintc/point.h"

// Util definitions

static point_t get_point(u32 index, u32 size, u32 total_index);

// Examples implementation

size_t japan_example(void)
{
    uSize height = 800;
    uSize width = height * 3 / 2;
    uSize dotSize = height * 3 / 5;
    canvas_t canvas;
	if (!krintc_alloc_data(&canvas, width, height)) return -1;

    krintc_fill(canvas, 0xFFFFFFFF);
    krintc_fill_circle(canvas, (i32)(width / 2), (i32)(height / 2), (i32)(dotSize / 2), 0xFF0000FF);

    if (!krintc_save_disk_image(canvas, "japan.png"))
    {
        printf("Failed to save japan.png!\n");
    }

    krintc_free_data(&canvas);
    return 0;
}

size_t line_example(void)
{
    uSize width = 200;
    uSize height = 200;
    canvas_t canvas;
	if (!krintc_alloc_data(&canvas, width, height)) return -1;

    krintc_fill(canvas, 0xFF000000);
    
    point_t basePosPoint = point_create(100, 100);
    for (u32 index = 0; index < 18; index++)
    {
        point_t current = get_point(index, 80, 18);
        point_t next = get_point((index + 1) % 18, 80, 18);
        point_add_self_point(&current, basePosPoint);
        point_add_self_point(&next, basePosPoint);

        krintc_line(canvas, 100, 100, current.x, current.y, index % 2 ? 0xFFFF0000 : 0xFF00FFFF);
        krintc_line(canvas, current.x, current.y, next.x, next.y, 0xFFFFFF00);
    }    

    if (!krintc_save_disk_image(canvas, "lines.png"))
    {
        printf("Failed to save lines.png!\n");
    }

    krintc_free_data(&canvas);
    return 0;
}

size_t triangle_example(void)
{
	uSize width = 800;
	uSize height = 800;
    canvas_t canvas;
	if (!krintc_alloc_data(&canvas, width, height)) return -1;

	krintc_fill(canvas, 0xFF000000);

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

		krintc_fill_triangle(canvas, w2, h2, current.x, current.y, next.x, next.y, index % 2 ? 0xFF00AAAA : 0xFF00FFFF);
	}
	
	if (!krintc_save_disk_image(canvas, "triangle.png"))
	{
		printf("Failed to save triangle.png!\n");
	}

	krintc_free_data(&canvas);
	return 0;
}

size_t blend_example(void)
{
	uSize width = 400;
	uSize height = 400;
    canvas_t canvas;
	if (!krintc_alloc_data(&canvas, width, height)) return -1;

	krintc_fill(canvas, 0xFF000000);

	krintc_fill_rect(canvas, 0, 0, 300, 300, 0xFF00FF00);
	krintc_fill_rect(canvas, 100, 100, 400, 400, 0xAA0000FF);
	krintc_fill_circle(canvas, 100, 100, 50, 0x77FF0000);
	krintc_fill_triangle(canvas, 50, 100, 0, 140, 120, 350, 0xCCFF00FF);

	if (!krintc_save_disk_image(canvas, "alphablend.png"))
	{
		printf("Failed to save alphablend.png!\n");
	}

	krintc_free_data(&canvas);
	return 0;
}

size_t text_example(void)
{
	uSize width = 40 + 26 * 4;
	uSize height = 40;
    canvas_t canvas;
	if (!krintc_alloc_data(&canvas, width, height)) return -1;

	krintc_fill(canvas, 0xFF000000);

	krintc_text(canvas, "abcdefghijklmnopqrstuvwxyz", 17, 20, 0xFFFFFFFF);

	if (!krintc_save_disk_image(canvas, "textdraw.png"))
	{
		printf("Failed to save textdraw.png!\n");
	}

	krintc_free_data(&canvas);
	return 0;
}

// Util implementations

static point_t get_point(u32 index, u32 size, u32 total_index)
{
    f32 angleBase = 3.141592f / ((f32)total_index / 2.0f);
    f32 angle = angleBase * index;
    f32 x = cosf(angle);
    f32 y = sinf(angle);

    return point_create((i32)(x * size), (i32)(y * size));
}



