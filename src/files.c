#include "files.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <string.h>

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
        file = malloc(len + strlen(filename) + 1);
        strcpy(file, dirname);
    }
    else
    {
        file = malloc(len + strlen(filename) + 2);
        strcpy(file, dirname);
        strcat(file, "/");
    }

    strcat(file, filename);

    return file;
}