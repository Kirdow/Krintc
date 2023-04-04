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

#include "files.h"
#include "mem.h"

static tests_t *tests_ptr = NULL;

tests_t *tests_load()
{
    if (tests_ptr) return tests_ptr;

    const args_t *args = args_get();
    if (!args->test_dir) return NULL;

    tests_t *ptr = (tests_t*)mem_alloc(sizeof(tests_t));
    ptr->test_dir = args->test_dir;
    
    ptrlist_t *data = ptrlist_create();

#ifdef WIN32
    char *dir_path = file_path_concat(ptr->test_dir, "*.png");
    WIN32_FIND_DATA file_data;
    HANDLE handle = FindFirstFile(dir_path, &file_data);

    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Error: tests directory not found\n");
        exit(1);
    }

    do {
        char *fname = mem_alloc(strlen(file_data.cFileName) + 1);
        strcpy(fname, file_data.cFileName);
        ptrlist_add_element(data, fname);
    } while (FindNextFile(handle, &file_data) != 0);

    FindClose(handle);
#else
    const char *dir_path = ptr->test_dir;
	DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        printf("Error: tests directory not found\n");
        exit(1);
    }

    struct dirent *entry;
    struct stat info;

	while ((entry = readdir(dir)) != NULL)
    {
		if (strstr(entry->d_name, ".png") != NULL) {
			if (stat(entry->d_name, &info) != 0) {
				printf("error stat: %s\n", entry->d_name);
				return NULL;
			}
			if (S_ISREG(info.st_mode)) {
				char *fname = mem_alloc(strlen(entry->d_name) + 1);
				strcpy(fname, entry->d_name);
				ptrlist_add_element(data, fname);
			}
		}
    }

    closedir(dir);
#endif
    ptr->data = data;

    tests_ptr = ptr;

#ifdef WIN32
    mem_free(dir_path);
#endif

    return tests_ptr;
}

void tests_free()
{
    if (!tests_ptr) return;

    ptrlist_t *data = tests_ptr->data;
    while (data->len > 0)
    {
        char *str = ptrlist_pop_type(char *, data);
        mem_free(str);
    }

    ptrlist_free(&tests_ptr->data);
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
