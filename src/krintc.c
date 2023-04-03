#include "krintc.h"
#include "vendor/stb_image_write.h"

void krintc_fill(u32 *pixels, uSize pixel_width, uSize pixel_height, u32 color)
{
    for (uSize i = 0; i < pixel_width * pixel_height; i++)
    {
        pixels[i] = ACOLOR(color);
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

            pixels[pixel_width * yp + xp] = ACOLOR(color); 
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

            pixels[pixel_width * yp + xp] = ACOLOR(color);
        }
    }
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

            i32 y = y0 + (x - x0) * dy / dx;
            if (y < 0 || y >= (i32)pixel_height) continue;
            uSize yp = (uSize)y;

            pixels[pixel_width * yp + xp] = ACOLOR(color);
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

            i32 x = x0 + (y - y0) * dx / dy;
            if (x < 0 || x >= (i32)pixel_width) continue;
            uSize xp = (uSize)x;

            pixels[pixel_width * yp + xp] = ACOLOR(color);
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

    pixels[pixel_width * yp + xp] = color;
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
    u32 *data = (u32*)malloc(sizeof(u32) * pixel_width * pixel_height);
    if (data == NULL) return false;

    *pixels = data;
    return true;
}

uBool krintc_free_data(u32 **pixels)
{
    if (pixels == NULL) return false;
    u32 *data = *pixels;
    if (data == NULL) return true;
    free(data);
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

