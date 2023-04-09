#include "tests.h"
#include "args.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#else
#define _DEFAULT_SOURCE
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "shared/files.h"
#include "shared/mem.h"

static tests_t *tests_ptr = NULL;

tests_t *tests_load()
{
    if (tests_ptr) return tests_ptr;

    const args_t *args = args_get();
    if (!args->test_dir) return NULL;

    tests_t *ptr = (tests_t*)mem_alloc(sizeof(tests_t));
    ptr->test_dir = args->test_dir;
   
	ptrlist_t *data = file_list_dir(ptr->test_dir, ".png");
    ptr->data = data;

    tests_ptr = ptr;

    return tests_ptr;
}

void tests_free()
{
    if (!tests_ptr) return;

    file_list_dir_free(&tests_ptr->data);
    mem_free(tests_ptr);
    tests_ptr = NULL;
}

const char *tests_get_file(size_t index)
{
    if (!tests_ptr) return NULL;

    return ptrlist_get_type(const char *, tests_ptr->data, index);
}

size_t tests_get_file_count()
{
    if (!tests_ptr) return 0;

    return tests_ptr->data->len;
}
