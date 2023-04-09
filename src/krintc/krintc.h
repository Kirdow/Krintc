#ifndef _KRINT_C_
#define _KRINT_C_

#include <stdio.h>
#include <stdlib.h>

#include "krintdef.h"
#include "canvas.h"

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
 * param canvas - buffer canvas
 * param color - color to fill
 */
void krintc_fill(canvas_t canvas, u32 color);

/*
 * fill a rect inside the buffer with a specific color
 * param canvas - buffer canvas
 * param x0, y0 - minimum rect point
 * param x1, y1 - maximum rect point
 * param color - color to fill
 */
void krintc_fill_rect(canvas_t canvas, i32 x0, i32 y0, i32 x1, i32 y1, u32 color);

/*
 * fill a circle inside the buffer with a specific color
 * param canvas - buffer canvas
 * param xc, yc - circle centre point
 * param radius - circle radius (in pixels)
 * param color - color to fill
 */
void krintc_fill_circle(canvas_t canvas, i32 xc, i32 yc, i32 radius, u32 color);

/*
 * fill a triangle inside the buffer with a specific color
 * param canvas - buffer canvas
 * param x0, y0 - first corner
 * param x1, y1 - second corner
 * param x2, y2 - third corner
 * param color - color to fill
 */
void krintc_fill_triangle(canvas_t canvas, i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u32 color);

/*
 * draw a line inside the buffer with a specific color
 * param canvas - buffer canvas
 * param x0, y0 - line start point
 * param x1, y1 - line end point
 * param color - color to draw
 */
void krintc_line(canvas_t canvas, i32 x0, i32 y0, i32 x1, i32 y1, u32 color);

/*
 * plot a single pixel inside the buffer with a specific color
 * param canvas - buffer canvas
 * param x0, y0 - pixel point
 * param color - color to plot
 */
void krintc_plot(canvas_t canvas, i32 x0, i32 y0, u32 color);

/*
 * draw text inside the buffer with a specific color
 * param canvas - buffer canvas
 * param text - text to draw
 * param x0, y0 - text point
 * param color - color to draw
 */
void krintc_text(canvas_t canvas, const char *text, i32 x0, i32 y0, u32 color);

/*
 * split a pixel into individual channels
 * param pixel - input pixel
 * param count - pixel channel count
 * param channels - pixel channels pointer
 */
void krintc_explode_color(u32 pixel, uSize count, u32 *channels);

/*
 * combine pixel channels into a single pixel
 * param pixel - output pixel
 * param count - pixel channel count
 * param channels - pixel channels pointer
 */
void krintc_implode_color(u32 *pixel, uSize count, u32 *channels);

/*
 * allocate a buffer of pixels compatible with Krint.c
 * param canvas - buffer canvas output pointer
 * param pixel_width, pixel_height - input buffer dimensions
 */
uBool krintc_alloc_data(canvas_t *canvas, uSize pixel_width, uSize pixel_height);

/*
 * free up a buffer allocated using `krintc_alloc_data`
 * param canvas - buffer canvas to free up
 */
uBool krintc_free_data(canvas_t *canvas);

/*
 * save image to disk to a specified path
 * param canvas - buffer canvas
 * param filename - target filepath for image .png file
 */
uBool krintc_save_disk_image(const canvas_t canvas, const c8 *filename);

#endif // _KRINT_C_
