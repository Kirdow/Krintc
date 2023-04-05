#include "point.h"

point_t point_create(i32 x, i32 y)
{
    point_t point;
    point.x = x;
    point.y = y;
    return point;
}

void point_add_self_point(point_t *point, point_t other)
{
    point->x += other.x;
    point->y += other.y;
}

void point_add_self_raw(point_t *point, i32 x, i32 y)
{
    point->x += x;
    point->y += y;
}

point_t point_add_point(point_t p0, point_t p1)
{
    point_t r;
    r.x = p0.x + p1.x;
    r.y = p0.y + p1.y;
    return r;
}

point_t point_add_raw(point_t p0, i32 x, i32 y)
{
    point_t r;
    r.x = p0.x + x;
    r.y = p0.y + y;
    return r;
}

void point_scale_self_raw(point_t *point, i32 scale)
{
    point->x *= scale;
    point->y *= scale;
}

point_t point_scale_raw(point_t p0, i32 scale)
{
    point_t r;
    r.x = p0.x * scale;
    r.y = p0.y * scale;
    return r;
}
