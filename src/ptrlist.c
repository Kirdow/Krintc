#include "ptrlist.h"

#include <stdlib.h>
#include <stdbool.h>

ptrlist_t *ptrlist_create()
{
    ptrlist_t *ptr = (ptrlist_t*)malloc(sizeof(ptrlist_t));
    ptr->data = NULL;
    ptr->len = 0;
    return ptr;
}

ptrlist_t *ptrlist_create_count(size_t count)
{
    ptrlist_t *ptr = (ptrlist_t*)malloc(sizeof(ptrlist_t));
    if (!ptr) return NULL;

    if (!ptrlist_alloc_count(ptr, count))
    {
        free(ptr);
        return NULL;
    }

    return ptr;
}

void ptrlist_free(ptrlist_t **ptr)
{
    if (ptr == NULL) return;

    ptrlist_t *pptr = *ptr;
    if (pptr == NULL) return;

    if (pptr->data) free(pptr->data);
    free(pptr);

    *ptr = NULL;
}

bool ptrlist_alloc_count(ptrlist_t *ptr, size_t count)
{
    if (ptr->data)
    {
        free(ptr->data);
        ptr->data = NULL;
        ptr->len = 0;
    }

    if (count == 0) return true;

    void** data = (void**)malloc(sizeof(void*) * count);
    if (!data) return false;
    ptr->data = data;
    for (size_t i = 0; i < count; i++) ptr->data[i] = NULL;
    ptr->len = count;
    return true;
}

bool ptrlist_realloc_count(ptrlist_t *ptr, size_t count)
{
    if (count == 0)
    {
        if (ptr->data) free(ptr->data);
        ptr->data = NULL;
        ptr->len = 0;
        return true;
    }

    void **data = (void**)realloc(ptr->data, sizeof(void*) * count);
    if (!data) return false;

    ptr->data = data;
    ptr->len = count;

    return true;
}

void *ptrlist_get_element(const ptrlist_t *ptr, size_t index)
{
    if (index >= ptr->len) return NULL;

    return ptr->data[index];
}

bool ptrlist_set_element(ptrlist_t *ptr, size_t index, void *value)
{
    if (index >= ptr->len) return false;

    ptr->data[index] = (char*)value;
    return true;
}

bool ptrlist_add_element(ptrlist_t *ptr, void *value)
{
    if (!ptrlist_realloc_count(ptr, ptr->len + 1)) return false;
    if (!ptrlist_set_element(ptr, ptr->len - 1, value)) return false;

    return true;
}

bool ptrlist_insert_element(ptrlist_t *ptr, size_t index, void *value)
{
    if (index >= ptr->len)
    {
        return ptrlist_add_element(ptr, value);
    }

    size_t len = ptr->len;
    if (!ptrlist_realloc_count(ptr, len + 1)) return false;
    for (size_t i = len - 1; i >= index; --i)
    {
        ptr->data[i+1] = ptr->data[i];
    }
    ptr->data[index] = value;

    return true;
}

void *ptrlist_remove_element(ptrlist_t *ptr, size_t index)
{
    if (index >= ptr->len)
    {
        return NULL;
    }

    void *element = ptr->data[index];
    for (size_t i = index; i < ptr->len - 1; ++i)
    {
        ptr->data[i] = ptr->data[i + 1];
    }

    ptrlist_realloc_count(ptr, ptr->len - 1);
    return element;
}

void *ptrlist_pop_element(ptrlist_t *ptr)
{
    if (ptr->len == 0) return NULL;
    void *element = ptr->data[ptr->len - 1];
    ptrlist_realloc_count(ptr, ptr->len - 1);
    return element;
}