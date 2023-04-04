#include "krintc.h"
#include "vendor/stb_image_write.h"
#include "mem.h"

#define KRINTC_COLOR_SPLIT(COLOR, INDEX) ((COLOR) >> ((INDEX)*8)) & 0xFF
#define KRINTC_COLOR_MERGE(CHAN, INDEX) (((CHAN) & 0xFF) << ((INDEX)*8))

static u32 krintc_blend_alpha(u32 c0, u32 c1)
{
	u32 r0 = KRINTC_COLOR_SPLIT(c0, 0);
	u32 g0 = KRINTC_COLOR_SPLIT(c0, 1);
	u32 b0 = KRINTC_COLOR_SPLIT(c0, 2);
	u32 a0 = KRINTC_COLOR_SPLIT(c0, 3);

	u32 r1 = KRINTC_COLOR_SPLIT(c1, 0);
	u32 g1 = KRINTC_COLOR_SPLIT(c1, 1);
	u32 b1 = KRINTC_COLOR_SPLIT(c1, 2);
	u32 a1 = KRINTC_COLOR_SPLIT(c1, 3);

	u32 r2 = r0 + (r1 - r0) * a1 / 255;
	u32 g2 = g0 + (g1 - g0) * a1 / 255;
	u32 b2 = b0 + (b1 - b0) * a1 / 255;
	u32 a2 = a0;

	return KRINTC_COLOR_MERGE(a2, 3) | KRINTC_COLOR_MERGE(b2, 2) | KRINTC_COLOR_MERGE(g2, 1) | KRINTC_COLOR_MERGE(r2, 0);
}

void krintc_fill(u32 *pixels, uSize pixel_width, uSize pixel_height, u32 color)
{
    for (uSize i = 0; i < pixel_width * pixel_height; i++)
    {
        pixels[i] = color;
    }
}

void krintc_fill_rect(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, u32 color)
{
    for (i32 y = y0; y < y1; y++)
    {
        if (y < 0 || y >= (i32)pixel_height) continue;
        uSize yp = (uSize)y;

        for (i32 x = x0; x < x1; x++)
        {
            if (x < 0 || x >= (i32)pixel_height) continue;
            uSize xp = (uSize)x;

            pixels[pixel_width * yp + xp] = krintc_blend_alpha(pixels[pixel_width * yp + xp], color);
        }
    }
}

void krintc_fill_circle(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 xc, i32 yc, i32 radius, u32 color)
{
    i32 x0 = xc - radius;
    i32 x1 = xc + radius;
    i32 y0 = yc - radius;
    i32 y1 = yc + radius;
    uSize r2 = (uSize)(radius * radius);

    for (i32 y = y0; y <= y1; y++)
    {
        if (y < 0 || y >= (i32)pixel_height) continue;
        uSize yp = (uSize)y;
        uSize yr = (uSize)(y - yc);
        yr *= yr;

        for (i32 x = x0; x <= x1; x++)
        {
            if (x < 0 || x >= (i32)pixel_width) continue;
            uSize xp = (uSize)x;
            uSize xr = (uSize)(x - xc);
            xr *= xr;

            if (xr + yr > r2) continue;

            pixels[pixel_width * yp + xp] = krintc_blend_alpha(pixels[pixel_width * yp + xp], color);
        }
    }
}

static i32 safe_div(i32 a, i32 b, i32 c)
{
	if (c == 0)
	{
		return a;
	}

	return a * b / c;
}

void krintc_fill_triangle(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color)
{
	if (y1 < y0)
	{
		KRINTC_SWAP(i32, y0, y1);
		KRINTC_SWAP(i32, x0, x1);
	}

	if (y2 < y1)
	{
		KRINTC_SWAP(i32, y1, y2);
		KRINTC_SWAP(i32, x1, x2);
	}
	
	i32 x3 = x0 + safe_div(x2 - x0, y1 - y0, y2 - y0);
	i32 y3 = y0 + safe_div(y2 - y0, y1 - y0, y2 - y0);

	if (y0 == y2)
	{
		KRINTC_SWAP(i32, x0, x1);
		KRINTC_SWAP(i32, y0, y1);

			
		x3 = x0 + safe_div(x2 - x0, y1 - y0, y2 - y0);
		y3 = y0 + safe_div(y2 - y0, y1 - y0, y2 - y0);
	}

	for (i32 i = y0; i <= y3; ++i)
	{
		i32 xLeft = x0 + safe_div(x2 - x0, i - y0, y2 - y0);
		i32 xRight = x0 + safe_div(x1 - x0, i - y0, y1 - y0);
		i32 y = y0 + safe_div(y2 - y0, i - y0, y2 - y0);
		if (xLeft > xRight) KRINTC_SWAP(i32, xLeft, xRight);

		krintc_line(pixels, pixel_width, pixel_height, xLeft, y, xRight - 1, y, color);
	}

	for (i32 i = y3; i <= y2; ++i)
	{
		i32 xLeft = x3 + safe_div(x2 - x3, i - y3, y2 - y3);
		i32 xRight = x1 + safe_div(x2 - x1, i - y3, y2 - y3);
		i32 y = y3 + safe_div(y2 - y3, i - y3, y2 - y3);
		if (xLeft > xRight) KRINTC_SWAP(i32, xLeft, xRight);

		krintc_line(pixels, pixel_width, pixel_height, xLeft, y, xRight - 1, y, color);
	}

#if 0 // for debugging triangles
	krintc_line(pixels, pixel_width, pixel_height, x0, y0, x2, y2, 0xFF0000FF);
	krintc_line(pixels, pixel_width, pixel_height, x0, y0, x1, y1, 0xFF00FF00);
	krintc_line(pixels, pixel_width, pixel_height, x1, y1, x2, y2, 0xFFFF0000);

	krintc_fill_circle(pixels, pixel_width, pixel_height, x3, y3, 6, 0xFFFF00FF);

	krintc_fill_circle(pixels, pixel_width, pixel_height, x0, y0, 4, 0xFF0000FF);
	krintc_fill_circle(pixels, pixel_width, pixel_height, x1, y1, 4, 0xFF00FF00);
	krintc_fill_circle(pixels, pixel_width, pixel_height, x2, y2, 4, 0xFFFF0000);
#endif
}

void krintc_line(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, u32 color)
{
    i32 dx = x1 - x0;
    i32 dy = y1 - y0;

    if (KRINTC_ABS(i32, dx) >= KRINTC_ABS(i32, dy))
    {
        if (x0 > x1)
        {
            KRINTC_SWAP(int, x0, x1);
            KRINTC_SWAP(int, y0, y1);
        }

        for (i32 x = x0; x <= x1; x++)
        {
            if (x < 0 || x >= (i32)pixel_width) continue;
            uSize xp = (uSize)x;

			i32 y = y0 + ((dx == 0) ? 0 : ((x - x0) * dy / dx));
            if (y < 0 || y >= (i32)pixel_height) continue;
            uSize yp = (uSize)y;

            pixels[pixel_width * yp + xp] = krintc_blend_alpha(pixels[pixel_width * yp + xp], color);
        }
    }
    else if (KRINTC_ABS(i32, dx) < KRINTC_ABS(i32, dy))
    {
        if (y0 > y1)
        {
            KRINTC_SWAP(int, x0, x1);
            KRINTC_SWAP(int, y0, y1);
        }

        for (i32 y = y0; y <= y1; y++)
        {
            if (y < 0 || y >= (i32)pixel_height) continue;
            uSize yp = (uSize)y;

			i32 x = x0 + ((dy == 0) ? 0 : ((y - y0) * dx / dy));
            if (x < 0 || x >= (i32)pixel_width) continue;
            uSize xp = (uSize)x;

            pixels[pixel_width * yp + xp] = krintc_blend_alpha(pixels[pixel_width * yp + xp], color);
        }
    }
    else
    {
        if (KRINTC_ABS(i32, dx) < KRINTC_ABS(i32, dy)) NO_IMPL("dx < dy");
        else NO_IMPL("dx = dy");
    }
}

void krintc_plot(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, u32 color)
{
    if (x0 < 0 || x0 >= (i32)pixel_width) return;
    if (y0 < 0 || y0 >= (i32)pixel_height) return;

    uSize xp = (uSize)x0;
    uSize yp = (uSize)y0;

    pixels[pixel_width * yp + xp] = krintc_blend_alpha(pixels[pixel_width * yp + xp], color);
}

void krintc_explode_rgb(u32 pixel, u32 *red, u32 *green, u32 *blue)
{
    if (red != NULL) *red = (pixel>>(0*8)) & 0xFF;
    if (green != NULL) *green = (pixel>>(1*8)) & 0xFF;
    if (blue != NULL) *blue = (pixel>>(2*8)) & 0xFF;
}

void krintc_implode_rgb(u32 *pixel, u32 red, u32 green, u32 blue)
{
    if (pixel == NULL) return;

    *pixel = ACOLOR(blue << (2*8) | green << (1*8) | red << (0*8));
}

uBool krintc_alloc_data(u32 **pixels, uSize pixel_width, uSize pixel_height)
{
    u32 *data = (u32*)mem_alloc(sizeof(u32) * pixel_width * pixel_height);
    if (data == NULL) return false;

    *pixels = data;
    return true;
}

uBool krintc_free_data(u32 **pixels)
{
    if (pixels == NULL) return false;
    u32 *data = *pixels;
    if (data == NULL) return true;
    mem_free(data);
    *pixels = NULL;
    return true;
}

uBool krintc_save_disk_image(const u32 *pixels, uSize pixel_width, uSize pixel_height, const c8 *filename)
{
    if (!stbi_write_png(filename, (int)pixel_width, (int)pixel_height, 4, pixels, sizeof(uint32_t) * pixel_width))
    {
        return false;
    }

    return true;
}

