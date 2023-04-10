#include "record.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "shared/ptrlist.h"
#include "shared/files.h"

#include "shared/mem.h"

static bool file_copy_into(const char *srcdir, const char *dstdir, const char *filename, const char *filename_dst);

void tests_record(const args_t *args)
{
	const char *suffix = "_expected";
	ptrlist_t *image_files = file_list_dir(".", ".png");
	size_t image_count = file_list_size(image_files);
	for (size_t file_index = 0; file_index < image_count; file_index++)
	{
		const char *filename = file_list_get(image_files, file_index);
		size_t len = strlen(filename) + strlen(suffix) + 1;
		char *filename_expected = mem_alloc(len);
		memset(filename_expected, 0, sizeof(char) * len);
		strncpy(filename_expected, filename, strlen(filename) - 4);
		strcat(filename_expected, suffix);
		strcat(filename_expected, ".png");
		if (!file_copy_into(".", args->test_dir, filename, filename_expected))
		{
			char *dstpath = file_path_concat(args->test_dir, filename_expected);
			printf("Failed to copy file './%s' into '%s'!\n", filename, dstpath);
			mem_free(dstpath);
		}
		mem_free(filename_expected);
	}

	file_list_dir_free(&image_files);
}

static bool file_copy_into(const char *srcdir, const char *dstdir, const char *filename, const char *filename_dst)
{
	if (filename_dst == NULL)
		filename_dst = filename;

    char *srcfile = file_path_concat(srcdir, filename);
    char *dstfile = file_path_concat(dstdir, filename_dst);

    if (!file_exists(srcfile))
    {
        mem_free(srcfile);
        mem_free(dstfile);

        return false;
    }

    bool result = file_copy(srcfile, dstfile);

    mem_free(srcfile);
    mem_free(dstfile);

    return result;
}
