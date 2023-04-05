#ifndef _TESTS_H_
#define _TESTS_H_

#include "shared/ptrlist.h"

typedef struct {
    ptrlist_t *data;
    const char* test_dir;
} tests_t;

tests_t *tests_load();
void tests_free();
const char *tests_get_file(size_t index);
size_t tests_get_file_count();

#endif // _TESTS_H_
