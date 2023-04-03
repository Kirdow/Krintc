#ifndef _POINT_H_
#define _POINT_H_

#include "krintdef.h"

typedef struct {
    i32 x, y;
} point_t;

point_t point_create(i32 x, i32 y);
void point_add_self_point(point_t *point, point_t other);
void point_add_self_raw(point_t *point, i32 x, i32 y);
point_t point_add_point(point_t p0, point_t p1);
point_t point_add_raw(point_t p0, i32 x, i32 y);
void point_scale_self_raw(point_t *point, i32 scale);
point_t point_scale_raw(point_t p0, i32 scale);

#endif // _POINT_H_