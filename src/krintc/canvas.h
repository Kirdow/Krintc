/*
 * This is a header only canvas implementation.
 * In order to use, create `canvas.c` and include `canvas.h` like this:
 *
 * #define KRINTC_CANVAS_IMPL
 * #include "canvas.h"
 *
 */

#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <string.h>
#include "krintdef.h"

typedef struct {
	u32 *pixels;
	uSize width;
	uSize height;
	uSize stride;
} canvas_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/*
 * initialize canvas from input data on an existing pointer
 * returns canvas_t* - same as first parameter
 * param canvas - input pointer to canvas to initialize
 * param pixels - input pixel array pointer
 * param width, height - input dimensions of the pixels
 * param stride - input full width of pixels in one row in the buffer
 */
canvas_t *krintc_canvas_self_create(canvas_t *canvas, u32 *pixels, uSize width, uSize height, uSize stride);

/*
 * retrieve canvas data from a canvas pointer into separate values
 * returns uBool - true if inputted canvas is valid
 * param canvas - input pointer to canvas to fetch from
 * param pixels - output pointer to pixel array pointer
 * param width, height - output pointer to buffer dimensions
 * param stride - output pointer to buffer stride
 */
uBool krintc_canvas_self_fetch(canvas_t *canvas, u32 **pixels, uSize *width, uSize *height, uSize *stride);

/*
 * initialize new stack-allocated canvas
 * returns canvas_t - resulting canvas_t data (copied)
 * param pixels - input pixel array pointer
 * param width, height - input dimensions of the pixels buffer
 * param stride - input full width of pixels in one row in the buffer
 */
canvas_t krintc_canvas_create(u32 *pixels, uSize width, uSize height, uSize stride);

/*
 * intitialize new sub-canvas from existing canvas
 *  existing canvas could also be a sub-canvas
 * returns canvas_t - resulting canvas_t data (copied)
 * param camvas - input pointer to bigger canvas
 * param x, y - input position in canvas where sub-canvas starts
 * param width, height - input dimension of sub-canvas
 */
canvas_t krintc_canvas_sub(canvas_t *canvas, uSize x, uSize y, uSize width, uSize height);

#ifdef KRINTC_CANVAS_IMPL

#include <stdio.h>

canvas_t *krintc_canvas_self_create(canvas_t *canvas, u32 *pixels, uSize width, uSize height, uSize stride)
{
	if (!canvas) return NULL;

	canvas->pixels = pixels;
	canvas->width = width;
	canvas->height = height;
	canvas->stride = stride;

	return canvas;
}

uBool krintc_canvas_self_fetch(canvas_t *canvas, u32 **pixels, uSize *width, uSize *height, uSize *stride)
{
	if (!canvas) return false;

	if (pixels) *pixels = canvas->pixels;
	if (width) *width = canvas->width;
	if (height) *height = canvas->height;
	if (stride) *stride = canvas->stride;

	return true;
}

canvas_t krintc_canvas_create(u32 *pixels, uSize width, uSize height, uSize stride)
{
	canvas_t canvas;
	krintc_canvas_self_create(&canvas, pixels, width, height, stride);

	return canvas;
}

canvas_t krintc_canvas_sub(canvas_t *canvas, uSize x, uSize y, uSize width, uSize height)
{
	canvas_t result;
	memset(&result, 0, sizeof(canvas_t));

	if (!canvas)
	{
		printf("WARN: krintc_canvas_sub; canvas is NULL\n");
		return result;
	}

	result.pixels = &canvas->pixels[canvas->stride * y + x];
	result.width = width;
	result.height = height;
	result.stride = canvas->stride;

	return result;
}

#endif // KRINTC_CANVAS_IMPL

#pragma GCC diagnostic pop

#endif // _CANVAS_H_
