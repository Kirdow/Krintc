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

/*
 * canvas_t implementation of canvas holding a u32 pixel buffer (supposedly BGR or ABGR),
 *  the dimensions, and a stride
 *
 * u32 *pixels - buffer of BGR or ABGR pixels at 8 bits per channel
 * uSize width - the width in full pixels
 * uSize height - the height in full pixels
 * uSize stride - the full length in buffer pixels to get one pixel down in the buffer
 * 
 * note: canvas_t is used both with main canvases and sub canvases.
 *  To instantiate a subcanvas, the pixels variable should point to (y * stride + x),
 *  and the dimensions should be of the new subcanvas. The stride however should
 *  remains the same as of the main/sub canvas it's derived upon.
 *
 * note: whenever you got a sub canvas derived upon another canvas, it's important
 *  not to free up the main canvas before the sub canvas, or otherwise not to use,
 *  the sub canvas after the canvas its based upon is free'd up.
 *
 * note: the canvas_t object itself and it's implementation in this file is not
 *  responsible for the freeing of the pixels variable. Whenever the object is destroyed,
 *  it's up to the owner of the canvas_t instance to call free on the pixels pointer.
 *  If this is a sub canvas, you're required to call free on the memory pointed to
 *  by the main canvas.
 */
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

/*
 * !! ATTENTION !!
 * This is the implementation for canvas_t.
 * There's no documentation beyond this point.
 * For proper documentation, see above this point
 */

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
