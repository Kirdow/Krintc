#include "mem.h"
#ifdef MEM_WRAP
#include <stdio.h>
#include <stdlib.h>

static size_t total_mem_size = 0;

void *_mem_alloc(size_t size)
{
    size_t *data = (size_t*)malloc(size + sizeof(size_t));
    *data = size;
#ifdef MEM_DEBUG
    printf("old total size: %lld\nnew total size: %lld\n", total_mem_size, total_mem_size + size);
#endif
    total_mem_size += size;
    return (void*)((char*)(data) + sizeof(size_t));
}

void *_mem_realloc(void *ptr, size_t size)
{
    if (ptr == NULL) return _mem_alloc(size);
    
    size_t *data = (size_t*)((char*)(ptr) - sizeof(size_t));
    size_t old_size = *data;

#ifdef MEM_DEBUG
    printf("old total size: %lld\n", total_mem_size);
#endif
    total_mem_size += size;
    total_mem_size -= old_size;
#ifdef MEM_DEBUG
    printf("new total size: %lld\n", total_mem_size);
#endif

    data = realloc((void*)data, size + sizeof(size_t));
    *data = size;
    return (void*)((char*)(data) + sizeof(size_t));
}

void _mem_free(void *ptr)
{
    size_t *data = (size_t*)ptr - 1;
    size_t old_size = *data;

    if (total_mem_size < old_size)
    {
        printf("Panic memory out of order!\n");
        exit(1);
    }

#ifdef MEM_DEBUG
    printf("old total size: %lld\n", total_mem_size);
#endif
    total_mem_size -= old_size;
#ifdef MEM_DEBUG
    printf("new total size: %lld\n", total_mem_size);
#endif

    free((void*)data);
}

void mem_validate(size_t size)
{
    if (size == (size_t)MEMV_ANY)
    {
        if (total_mem_size == 0)
        {
            printf("Memory status (good): 0x0!\n");
        }
        else
        {
            printf("Memory status: 0x%llx!\n", total_mem_size);
        }

        return;
    }
    else if (total_mem_size != size)
    {
        if (size == (size_t)MEMV_ANY)
        {
            printf("Memory status: 0x%llx!\n", total_mem_size);
            return;
        }
        else if (size == (size_t)MEMV_NONE)
        {
            printf("Panic memory mismatch: 0x%llx!\n", total_mem_size);
            exit(-1);
        }
        else if (size > 0)
        {
            if (total_mem_size < size)
            {
                printf("Memory mismatch: internal 0x%llx < expected 0x%llx!\n", total_mem_size, size);
                exit(-1);
            }
            else if (total_mem_size > size)
            {
                printf("Memory mismatch: internal 0x%llx > expected 0x%llx!\n", total_mem_size, size);
                exit(-1);
            }
        }
    }
    else if (size == (size_t)MEMV_ANY)
    {
        printf("Memory status (good): 0x%llx!\n", total_mem_size);
    }
}
#endif // MEM_WRAP