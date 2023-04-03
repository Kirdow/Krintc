#include "tests.h"
#include "args.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <dirent.h>
#endif

#include "files.h"

static tests_t *tests_ptr = NULL;

tests_t *tests_load()
{
    if (tests_ptr) return tests_ptr;

    const args_t *args = args_get();
    if (!args->test_dir) return NULL;

    tests_t *ptr = (tests_t*)malloc(sizeof(tests_t));
    ptr->test_dir = args->test_dir;
    
    ptrlist_t *data = ptrlist_create();

    char *dir_path = file_path_concat(ptr->test_dir, "*.png");
#ifdef WIN32
    WIN32_FIND_DATA file_data;
    HANDLE handle = FindFirstFile(dir_path, &file_data);

    if (handle == INVALID_HANDLE_VALUE)
    {
        printf("Error: tests directory not found\n");
        exit(1);
    }

    do {
        char *fname = malloc(strlen(file_data.cFileName) + 1);
        strcpy(fname, file_data.cFileName);
        ptrlist_add_element(data, fname);
    } while (FindNextFile(handle, &file_data) != 0);

    FindClose(handle);
#else
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        printf("Error: tests directory not found\n");
        exit(1);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && strcmp(entry->d_name + strlen(entry->d_name) - strlen(ext), ext) == 0)
        {
            char *fname = malloc(strlen(entry->d_name) + 1);
            strcpy(fname, entry->d_name);
            ptrlist_add_element(data, fname);
        }
    }

    closedir(dir);
#endif
    ptr->data = data;

    tests_ptr = ptr;

    free(dir_path);

    return tests_ptr;
}

void tests_free()
{
    if (!tests_ptr) return;

    ptrlist_t *data = tests_ptr->data;
    while (data->len > 0)
    {
        char *str = ptrlist_pop_type(char *, data);
        free(str);
    }

    tests_ptr->data = NULL;
    free(tests_ptr);
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