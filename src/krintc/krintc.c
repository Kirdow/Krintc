#include "krintc/krintc.h"
#include "vendor/stb_image_write.h"
#include "shared/mem.h"

#define KRINTC_COLOR_SPLIT(COLOR, INDEX) ((COLOR) >> ((INDEX)*8)) & 0xFF
#define KRINTC_COLOR_MERGE(CHAN, INDEX) (((CHAN) & 0xFF) << ((INDEX)*8))

#include "fontg.inc.h"

#define BLEND_CHANNELS 4
#define BLEND_ALPHA_CHAN 3

#define KRINTC_RED(c)	(((c)>>(0*8))&0xFF)
#define KRINTC_GREEN(c) (((c)>>(1*8))&0xFF)
#define KRINTC_BLUE(c)  (((c)>>(2*8))&0xFF)
#define KRINTC_ALPHA(c) (((c)>>(3*8))&0xFF)
#define KRINTC_ABGR(a, b, g, r) (((a)<<(3*8)) | ((b)<<(2*8)) | ((g)<<(1*8)) | ((r)<<(0*8)))
#define KRINTC_RGBA(r, g, b, a) KRINTC_ABGR(a, b, g, r)

static inline u32 krintc_blend_alpha_channel(u32 a0, u32 a1)
{
	return (a0 + a1) / 2;
}

static inline void krintc_blend_alpha(u32 *c0, u32 c1)
{
	u32 a0 = KRINTC_ALPHA(*c0);
	u32 b0 = KRINTC_BLUE(*c0);
	u32 g0 = KRINTC_GREEN(*c0);
	u32 r0 = KRINTC_RED(*c0);

	u32 a1 = KRINTC_ALPHA(c1);
	u32 b1 = KRINTC_BLUE(c1);
	u32 g1 = KRINTC_GREEN(c1);
	u32 r1 = KRINTC_RED(c1);

	b0 = (b0 * (255 - a1) + b1 * a1) / 255;
	g0 = (g0 * (255 - a1) + g1 * a1) / 255;
	r0 = (r0 * (255 - a1) + r1 * a1) / 255;

	*c0 = KRINTC_ABGR(a0, b0, g0, r0);
}

void krintc_fill(canvas_t canvas, u32 color)
{
	for (uSize y = 0; y < canvas.height; y++)
	{
		for (uSize x = 0; x < canvas.width; x++)
		{
			canvas.pixels[canvas.stride * y + x] = color;
		}
	}
}

void krintc_fill_rect(canvas_t canvas, i32 x0, i32 y0, i32 x1, i32 y1, u32 color)
{
    for (i32 y = y0; y < y1; y++)
    {
        if (y < 0 || y >= (i32)canvas.height) continue;
        uSize yp = (uSize)y;

        for (i32 x = x0; x < x1; x++)
        {
            if (x < 0 || x >= (i32)canvas.width) continue;
            uSize xp = (uSize)x;

            krintc_blend_alpha(&canvas.pixels[canvas.stride * yp + xp], color);
        }
    }
}

#define KRINTC_AA_RES 2
#define KRINTC_AA_RES_D (KRINTC_AA_RES*2+1)

static inline i32 kmin(i32 a, i32 b)
{
	return a < b ? a : b;
}

static inline i32 kmax(i32 a, i32 b)
{
	return a > b ? a : b;
}

static inline i32 kclamp(i32 v, i32 vmin, i32 vmax)
{
	return kmax(vmin, kmin(vmax, v));
}

void krintc_fill_circle(canvas_t canvas, i32 xc, i32 yc, i32 r, u32 color)
{
	i32 x0 = kmax(xc - r, 0);
	i32 x1 = kmin(xc + r, canvas.width - 1);
	i32 y0 = kmax(yc - r, 0);
	i32 y1 = kmin(yc + r, canvas.height - 1);

	u32 alpha = (color >> 24) & 0xFF;
	color &= 0xFFFFFF;
    for (i32 yp = y0; yp <= y1; yp++)
    {
        for (i32 xp = x0; xp <= x1; xp++)
        {
			u32 count = 0;
			for (i32 ay = -KRINTC_AA_RES; ay <= KRINTC_AA_RES; ay++)
			{
				for (i32 ax = -KRINTC_AA_RES; ax <= KRINTC_AA_RES; ax++)
				{
					i32 res = KRINTC_AA_RES;
					i32 iy = (yp - yc)*res + ay;
					i32 ix = (xp - xc)*res + ax;
					if (ix*ix + iy*iy <= r*r*res*res) ++count;
				}
			}

			if (!count) continue;

			u32 aa_alpha = count*alpha/KRINTC_AA_RES_D/KRINTC_AA_RES_D;
			krintc_blend_alpha(&canvas.pixels[canvas.stride * yp + xp], color | (aa_alpha << 24));
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

void krintc_fill_triangle(canvas_t canvas, i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color)
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

		krintc_line(canvas, xLeft, y, xRight - 1, y, color);
	}

	for (i32 i = y3 + 1; i <= y2; ++i)
	{
		i32 xLeft = x3 + safe_div(x2 - x3, i - y3, y2 - y3);
		i32 xRight = x1 + safe_div(x2 - x1, i - y3, y2 - y3);
		i32 y = y3 + safe_div(y2 - y3, i - y3, y2 - y3);
		if (xLeft > xRight) KRINTC_SWAP(i32, xLeft, xRight);

		krintc_line(canvas, xLeft, y, xRight - 1, y, color);
	}

#if 0 // for debugging triangles
	krintc_line(canvas, x0, y0, x2, y2, 0xFF0000FF);
	krintc_line(canvas, x0, y0, x1, y1, 0xFF00FF00);
	krintc_line(canvas, x1, y1, x2, y2, 0xFFFF0000);

	krintc_fill_circle(canvas, x3, y3, 6, 0xFFFF00FF);

	krintc_fill_circle(canvas, x0, y0, 4, 0xFF0000FF);
	krintc_fill_circle(canvas, x1, y1, 4, 0xFF00FF00);
	krintc_fill_circle(canvas, x2, y2, 4, 0xFFFF0000);
#endif
}

void krintc_line(canvas_t canvas, i32 x0, i32 y0, i32 x1, i32 y1, u32 color)
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
            if (x < 0 || x >= (i32)canvas.width) continue;
            uSize xp = (uSize)x;

			i32 y = y0 + ((dx == 0) ? 0 : ((x - x0) * dy / dx));
            if (y < 0 || y >= (i32)canvas.height) continue;
            uSize yp = (uSize)y;

            krintc_blend_alpha(&canvas.pixels[canvas.stride * yp + xp], color);
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
            if (y < 0 || y >= (i32)canvas.height) continue;
            uSize yp = (uSize)y;

			i32 x = x0 + ((dy == 0) ? 0 : ((y - y0) * dx / dy));
            if (x < 0 || x >= (i32)canvas.width) continue;
            uSize xp = (uSize)x;

            krintc_blend_alpha(&canvas.pixels[canvas.stride * yp + xp], color);
        }
    }
    else
    {
        if (KRINTC_ABS(i32, dx) < KRINTC_ABS(i32, dy)) NO_IMPL("dx < dy");
        else NO_IMPL("dx = dy");
    }
}

static void krintc_char(canvas_t canvas, char c, i32 x0, i32 y0, u32 color)
{
	for (i32 y = 0; y < FONT_HEIGHT; y++)
	{
		uSize yp = (uSize)(y + y0);
		if (yp >= canvas.height) continue;

		for (i32 x = 0; x < FONT_WIDTH; x++)
		{
			uSize xp = (uSize)(x + x0);
			if (xp >= canvas.width) continue;

			if (FONT_MAP[(uSize)(u8)c][(uSize)y][(uSize)x])
			{
				krintc_blend_alpha(&canvas.pixels[canvas.stride * yp + xp], color);
			}
		}
	}
}

void krintc_text(canvas_t canvas, const char *ptr, i32 x0, i32 y0, u32 color)
{
	i32 x = 0;
	i32 y = 0;

	while (*ptr != 0)
	{
		char c = *ptr;
		if (c == '\n')
		{
			x = 0;
			y += FONT_HEIGHT + 1;
		}
		else
		{
			krintc_char(canvas, c, x + x0, y + y0, color);
			x += FONT_WIDTH_MAP[(uSize)(u8)c] + 1;
		}

		ptr++;
	}
}

void krintc_plot(canvas_t canvas, i32 x0, i32 y0, u32 color)
{
    if (x0 < 0 || x0 >= (i32)canvas.width) return;
    if (y0 < 0 || y0 >= (i32)canvas.height) return;

    uSize xp = (uSize)x0;
    uSize yp = (uSize)y0;

    krintc_blend_alpha(&canvas.pixels[canvas.stride * yp + xp], color);
}

void krintc_explode_color(u32 pixel, uSize count, u32 *channels)
{
	for (uSize index = 0; index < count; index++)
	{
		channels[index] = (pixel >> (index * 8)) & 0xFF;
	}
}

void krintc_implode_color(u32 *pixel, uSize count, u32 *channels)
{
	u32 color = 0;

	for (uSize index = 0; index < count; index++)
	{
		color |= channels[index] << (index * 8);
	}

	*pixel = color;
}

uBool krintc_alloc_data(canvas_t *canvas, uSize pixel_width, uSize pixel_height)
{
	if (canvas == NULL) return false;

    u32 *data = (u32*)mem_alloc(sizeof(u32) * pixel_width * pixel_height);
    if (data == NULL) return false;

	canvas->pixels = data;
	canvas->width = pixel_width;
	canvas->height = pixel_height;
	canvas->stride = pixel_width;
    return true;
}

uBool krintc_free_data(canvas_t *canvas)
{
	if (canvas == NULL) return false;
	u32 *data = canvas->pixels;
	if (data == NULL) return true;
	mem_free(data);
	memset(canvas, 0, sizeof(canvas_t));
    return true;
}

uBool krintc_save_disk_image(const canvas_t canvas, const c8 *filename)
{
    if (!stbi_write_png(filename, (int)canvas.width, (int)canvas.height, 4, canvas.pixels, sizeof(uint32_t) * canvas.stride))
    {
        return false;
    }

    return true;
}

