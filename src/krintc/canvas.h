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

canvas_t *krintc_canvas_self_create(canvas_t *canvas, u32 *pixels, uSize width, uSize height, uSize stride);
uBool krintc_canvas_self_fetch(canvas_t *canvas, u32 **pixels, uSize *width, uSize *height, uSize *stride);
canvas_t krintc_canvas_create(u32 *pixels, uSize width, uSize height, uSize stride);
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
