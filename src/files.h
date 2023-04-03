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

bool file_copy(const char *srcname, const char *dstname);
bool file_delete(const char *filename);
char *file_path_concat(const char *dirname, const char *filename);

#define file_exists(fname) (access(fname, F_OK) == 0)


#endif