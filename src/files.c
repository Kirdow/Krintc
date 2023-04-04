#include "files.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>
#include "mem.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

bool file_copy(const char *srcname, const char *dstname)
{
    FILE *source, *dest;
    char buffer[1024];
    size_t bytes_read;

    source = fopen(srcname, "rb");
    if (!source) return false;

    dest = fopen(dstname, "wb");
    if (!dest)
    {
        fclose(source);
        return false;
    }

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)))
    {
        fwrite(buffer, 1, bytes_read, dest);
    }

    fclose(source);
    fclose(dest);

    return true;
}

bool file_delete(const char *filename)
{
    if (!file_exists(filename)) return false;

    return remove(filename) == 0;
}

char *file_path_concat(const char *dirname, const char *filename)
{
    char *file = NULL;
    size_t len = strlen(dirname);

    if (dirname[len - 1] == '/')
    {
        file = mem_alloc(len + strlen(filename) + 1);
        strcpy(file, dirname);
    }
    else
    {
        file = mem_alloc(len + strlen(filename) + 2);
        strcpy(file, dirname);
        strcat(file, "/");
    }

    strcat(file, filename);

    return file;
}

ptrlist_t *file_list_dir(const char *dirname, const char *ext)
{
	ptrlist_t *data = ptrlist_create();

#ifdef WIN32
	char *wext = NULL;
	if (ext == NULL)
	{
		ext = "*.*";
		wext = mem_alloc(strlen(ext) + 1);
		strcpy(wext, ext);
	}
	else
	{
		wext = mem_alloc(strlen(ext) + 2);
		strcpy(wext, "*");
		strcat(wext, ext);
	}

	char *dir_path = file_path_concat(dirname, wext);
	mem_free(wext);

	WIN32_FIND_DATA file_data;
	HANDLE handle = FindFirstFile(dir_path, &file_data);

	if (handle == INVALID_HANDLE_VALUE)
	{
		printf("Error: %s directory not found\n", dirname);
		file_list_dir_free(&data);
		mem_free(dir_path);
		return NULL;
	}

	do
	{
		char *fname = mem_alloc(strlen(file_data.cFileName) + 1);
		strcpy(fname, file_data.cFileName);
		ptrlist_add_element(data, fname);
	} while (FindNextFile(handle, &file_data) != 0);

	FindClose(handle);
	
	mem_free(dir_path);
#else
	const char *dir_path = dirname;
	DIR *dir = opendir(dir_path);
	if (dir == NULL)
	{
		printf("Error: %s directory not found\n", dirname);
		ptrlist_free(&data);
		return NULL;
	}

	struct dirent *entry;
	struct stat info;

	while ((entry = readdir(dir)) != NULL)
	{
		if (ext == NULL || strstr(entry->d_name, ext) != NULL)
		{
			char *fstatname = file_path_concat(dirname, entry->d_name);
			if (stat(fstatname, &info) != 0)
			{
				printf("error stat: %s\n", entry->d_name);
				mem_free(fstatname);
				ptrlist_free(&data);
				return NULL;
			}
			mem_free(fstatname);

			if (S_ISREG(info.st_mode))
			{
				char *fname = mem_alloc(strlen(entry->d_name) + 1);
				strcpy(fname, entry->d_name);
				ptrlist_add_element(data, fname);
			}
		}
	}

	closedir(dir);
#endif

	return data;
}

void file_list_dir_free(ptrlist_t **list)
{
	ptrlist_t *data = *list;
	while (data->len > 0)
	{
		char *str = ptrlist_pop_type(char *, data);
		mem_free(str);
	}

	ptrlist_free(list);
}

const char *file_list_get(ptrlist_t *ptr, size_t index)
{
	return ptrlist_get_type(const char *, ptr, index);
}

size_t file_list_size(ptrlist_t *ptr)
{
	return ptr->len;
}

