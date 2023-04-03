#ifndef _KRINT_C_
#define _KRINT_C_

#include "krintdef.h"
#include <stdio.h>
#include <stdlib.h>

#define ACOLOR(color) ((color) | 0xFF << (3*8));
#define KRINTC_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define KRINTC_ABS(T, x) (KRINTC_SIGN(T, x) * (x))
#define KRINTC_SWAP(T, x, y) do { T _tmp = x; x = y; y = _tmp; } while (0)
#define NO_IMPL(x) do { printf("WARN: Not Implemented! {%s}\n", x); } while (0)
#define RETURN_NO_IMPL(x) do { NO_IMPL(); return (x); } while (0)

void krintc_fill(u32 *pixels, uSize pixel_width, uSize pixel_height, u32 color);
void krintc_fill_rect(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, u32 color);
void krintc_fill_circle(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 xc, i32 yc, i32 radius, u32 color);
void krintc_line(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, u32 color);
void krintc_plot(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, u32 color);
void krintc_explode_rgb(u32 pixel, u32 *red, u32 *green, u32 *blue);
void krintc_implode_rgb(u32 *pixel, u32 red, u32 green, u32 blue);
uBool krintc_alloc_data(u32 **pixels, uSize pixel_width, uSize pixel_height);
uBool krintc_free_data(u32 **pixels);

#endif // _KRINT_C_
