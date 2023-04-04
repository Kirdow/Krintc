#ifndef _FILES_H_
#define _FILES_H_

#include <stdbool.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

#include "ptrlist.h"

bool file_copy(const char *srcname, const char *dstname);
bool file_delete(const char *filename);
char *file_path_concat(const char *dirname, const char *filename);

ptrlist_t *file_list_dir(const char *dirname, const char *ext);
void file_list_dir_free(ptrlist_t **list);

const char *file_list_get(ptrlist_t *ptr, size_t index);
size_t file_list_size(ptrlist_t *ptr);

#define file_exists(fname) (access(fname, F_OK) == 0)


#endif
