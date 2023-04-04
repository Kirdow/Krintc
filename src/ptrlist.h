#ifndef _PTRLIST_H_
#define _PTRLIST_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * ptrlist_t implementation of vector list to store list of pointers
 * 
 * note: ptrlist_t does not take ownership of any pointers stored in,
 *  the list. Upon using `ptrlist_remove_element` or `ptrlist_pop_element`
 *  it is up to the caller to free up the memory help by the returned
 *  pointer.
 */
typedef struct {
    void **data;
    size_t len;
} ptrlist_t;

/*
 * create new ptrlist_t instance
 * returns ptrlist_t pointer instance
 */
ptrlist_t *ptrlist_create();

/*
 * create new ptrlist_t with a pre-allocated size
 * returns ptrlist_t pointer instance
 * param count - pre-allocated item count
 */
ptrlist_t *ptrlist_create_count(size_t count);

/*
 * delete ptrlist_t instance
 * param ptr - pointer to ptrlist_t pointer instance
 */
void ptrlist_free(ptrlist_t **ptr);

/*
 * internal: allocate storage of size `count`
 * param ptr - ptrlist_t instance pointer
 * param count - size of list to allocate
 */
bool ptrlist_alloc_count(ptrlist_t *ptr, size_t count);

/*
 * internal: reallocate storage of size `count`
 * param ptr - ptrlist_t instance pointer
 * param count - new size of list to reallocate
 */
bool ptrlist_realloc_count(ptrlist_t *ptr, size_t count);

/*
 * get pointer element at index in list
 * returns pointer stored at index or NULL if index out of bounds
 * param ptr - ptrlist_t instance pointer
 * param index - index in the list
 */
void *ptrlist_get_element(const ptrlist_t *ptr, size_t index);

/*
 * set pointer element at index in list
 * returns bool for success state, false if index out of bounds
 * param ptr - ptrlist_t instance pointer
 * param index - index in the list
 * param value - pointer to store at index
 */
bool ptrlist_set_element(ptrlist_t *ptr, size_t index, void *value);

/*
 * add pointer element at end of list
 * returns bool for success state
 * param ptr - ptrlist_t instance pointer
 * param value - pointer to store
 */
bool ptrlist_add_element(ptrlist_t *ptr, void *value);

/*
 * insert pointer element at index in list
 * returns bool for success state, false if index out of bounds
 * param ptr - ptrlist_t instance pointer
 * param index - index in the list (note: insertion done before this index)
 * param value - pointer to store
 */
bool ptrlist_insert_element(ptrlist_t *ptr, size_t index, void *value);

/*
 * remove pointer element at index in list
 * returns removed pointer element (note: it's up to caller to free up memory)
 * param ptr - ptrlist_t instance pointer
 * param index - index in the list
 */
void *ptrlist_remove_element(ptrlist_t *ptr, size_t index);

/*
 * remove last pointer element in list
 * returns removed pointer element (note: it's up to caller to free up memory)
 * param ptr - ptrlist_t instance pointer
 */
void *ptrlist_pop_element(ptrlist_t *ptr);

/*
 * wrapper for `ptrlist_get_element` to cast to a type when returned
 */
#define ptrlist_get_type(T, ptr, index) ((T)(ptrlist_get_element(ptr, index)))

/*
 * wrapper for `ptrlist_remove_element` to cast to a type when returned
 */
#define ptrlist_remove_type(T, ptr, index) ((T)(ptrlist_remove_element(ptr, index)))

/*
 * wrapper for `ptrlist_pop_element` to cast to a type when returned
 */
#define ptrlist_pop_type(T, ptr) ((T)(ptrlist_pop_element(ptr)))

#endif // _PTRLIST_H_
