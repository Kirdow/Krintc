#ifndef _KRINT_C_
#define _KRINT_C_

#include "krintdef.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * add alpha to a 24-bit BGR pixel
 * note: turns 0xBBGGRR into 0xAABBGGRR with a fixed alpha of 0xFF
 */
#define ACOLOR(color) ((color) | 0xFF << (3*8));

/*
 * generic macros used for mathematical operations
 * macro KRINTC_SIGN - copy the sign of a unit
 * macro KRINTC_ABS - force a positive (x >= 0) value
 * macro KRINTC_SWAP - swap two values
 */
#define KRINTC_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define KRINTC_ABS(T, x) (KRINTC_SIGN(T, x) * (x))
#define KRINTC_SWAP(T, x, y) do { T _tmp = x; x = y; y = _tmp; } while (0)

/*
 * debug print for not yet implemented branches
 * param x - required text parameter for extra data
 */
#define NO_IMPL(x) do { printf("WARN: Not Implemented! {%s}\n", x); } while (0)

/*
 * debug print with return for not yet implemented branches
 * param x - value to return after printing
 */
#define RETURN_NO_IMPL(x) do { NO_IMPL(""); return (x); } while (0)

/*
 * clear the entire buffer with one color
 * param pixels, pixel_width, pixel_height - buffer definition
 * param color - color to fill
 */
void krintc_fill(u32 *pixels, uSize pixel_width, uSize pixel_height, u32 color);

/*
 * fill a rect inside the buffer with a specific color
 * param pixels, pixel_width, pixel_height - buffer definition
 * param x0, y0 - minimum rect point
 * param x1, y1 - maximum rect point
 * param color - color to fill
 */
void krintc_fill_rect(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, u32 color);

/*
 * fill a circle inside the buffer with a specific color
 * param pixels, pixel_width, pixel_height - buffer definition
 * param xc, yc - circle centre point
 * param radius - circle radius (in pixels)
 * param color - color to fill
 */
void krintc_fill_circle(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 xc, i32 yc, i32 radius, u32 color);

/*
 * fill a triangle inside the buffer with a specific color
 * param pixels, pixel_width, pixel_height - buffer definition
 * param x0, y0 - first corner
 * param x1, y1 - second corner
 * param x2, y2 - third corner
 * param color - color to fill
 */
void krintc_fill_triangle(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color);

/*
 * draw a line inside the buffer with a specific color
 * param pixels, pixel_width, pixel_height - buffer definition
 * param x0, y0 - line start point
 * param x1, y1 - line end point
 * param color - color to draw
 */
void krintc_line(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, i32 x1, i32 y1, u32 color);

/*
 * plot a single pixel inside the buffer with a specific color
 * param pixels, pixel_width, pixel_height - buffer definition
 * param x0, y0 - pixel point
 * param color - color to plot
 */
void krintc_plot(u32 *pixels, uSize pixel_width, uSize pixel_height, i32 x0, i32 y0, u32 color);

void krintc_text(u32 *pixels, uSize pixel_width, uSize pixel_height, const char *text, i32 x0, i32 y0, u32 color);

/*
 * split a pixel into individual channels
 * param pixel - input pixel in 0xBBGGRR format
 * param red, green, blue - output channels
 */
void krintc_explode_rgb(u32 pixel, u32 *red, u32 *green, u32 *blue);

/*
 * combine pixel channels into a single pixel
 * param pixel - output pixel in 0xBBGGRR format
 * param red, green, blue - input channels in 0x00 - 0xFF format
 */
void krintc_implode_rgb(u32 *pixel, u32 red, u32 green, u32 blue);

/*
 * allocate a buffer of pixels compatible with Krint.c
 * param pixels - output pixel buffer in 0xAABBGGRR format
 * param pixel_width, pixel_height - input buffer dimensions
 */
uBool krintc_alloc_data(u32 **pixels, uSize pixel_width, uSize pixel_height);

/*
 * free up a buffer allocated using `krintc_alloc_data`
 * param pixels - pixel buffer to free up
 */
uBool krintc_free_data(u32 **pixels);

/*
 * save image to disk to a specified path
 * param pixels, pixel_width, pixel_height - buffer definition
 * param filename - target filepath for image .png file
 */
uBool krintc_save_disk_image(const u32 *pixels, uSize pixel_width, uSize pixel_height, const c8 *filename);

#endif // _KRINT_C_
