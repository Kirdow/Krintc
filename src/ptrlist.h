#ifndef _PTRLIST_H_
#define _PTRLIST_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    void **data;
    size_t len;
} ptrlist_t;

ptrlist_t *ptrlist_create();
ptrlist_t *ptrlist_create_count(size_t count);
void ptrlist_free(ptrlist_t **ptr);
bool ptrlist_alloc_count(ptrlist_t *ptr, size_t count);
bool ptrlist_realloc_count(ptrlist_t *ptr, size_t count);
void *ptrlist_get_element(const ptrlist_t *ptr, size_t index);
bool ptrlist_set_element(ptrlist_t *ptr, size_t index, void *value);
bool ptrlist_add_element(ptrlist_t *ptr, void *value);
bool ptrlist_insert_element(ptrlist_t *ptr, size_t index, void *value);
void *ptrlist_remove_element(ptrlist_t *ptr, size_t index);
void *ptrlist_pop_element(ptrlist_t *ptr);

#define ptrlist_get_type(T, ptr, index) ((T)(ptrlist_get_element(ptr, index)))
#define ptrlist_remove_type(T, ptr, index) ((T)(ptrlist_remove_element(ptr, index)))
#define ptrlist_pop_type(T, ptr) ((T)(ptrlist_pop_element(ptr)))

#endif // _PTRLIST_H_
