#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

#define MEMV_NONE (0)
#define MEMV_ANY (-1)
#define MEMV_X(x) (x + 1)
#define _MEMV_NX(x) (x - 1)
#ifdef MEM_WRAP
void *_mem_alloc(size_t size);
void *_mem_realloc(void *ptr, size_t size);
void _mem_free(void *ptr);
void mem_validate(size_t size);

#ifndef MEM_DEBUG
#define mem_alloc(size) _mem_alloc(size)
#define mem_realloc(ptr, size) _mem_realloc(ptr, size)
#define mem_free(ptr) _mem_free(ptr)
#else
#include <stdio.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

static void *_mem_alloc_log(size_t size, const char *file, size_t line)
{
    void *rptr = _mem_alloc(size);
    printf("0x%llx = mem_alloc(%lld) in %s:%lld\n", (size_t)rptr, size, file, line);
    return rptr;
}

static void *_mem_realloc_log(void *ptr, size_t size, const char *file, size_t line)
{
    void *rptr = _mem_realloc(ptr, size);
    printf("0x%llx = mem_realloc(0x%llx, %lld) in %s:%lld\n", (size_t)rptr, (size_t)ptr, size, file, line);
    return rptr;
}

static void _mem_free_log(void *ptr, const char *file, size_t line)
{
    _mem_free(ptr);
    printf("mem_free(0x%llx) in %s:%lld\n", (size_t)ptr, file, line);
}

#define mem_alloc(size) _mem_alloc_log(size, __FILE__, __LINE__)
#define mem_realloc(ptr, size) _mem_realloc_log(ptr, size, __FILE__, __LINE__)
#define mem_free(ptr) _mem_free_log(ptr, __FILE__, __LINE__)

#pragma GCC diagnostic pop
#endif // MEM_DEBUG
#else

#include <stdlib.h>
#define mem_alloc(size) malloc(size)
#define mem_realloc(ptr, size) realloc(ptr, size)
#define mem_free(ptr) free(ptr)
#define mem_validate(size)

#endif // MEM_WRAP


#endif // _MEM_H_