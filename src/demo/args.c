#include "args.h"
#include "shared/strutil.h"
#include "shared/mem.h"

#include <stdlib.h>
#define NEXTARG(x) do { i += 1; if (i >= argc) break; x = argv[i]; } while (0);

static args_t *args = NULL;

const args_t *args_get()
{
    return args;
}

void args_free()
{
    if (!args) return;

    mem_free(args);
    args = NULL;
}

void args_load(int argc, const char *argv[])
{
    if (args)
    {
        mem_free(args);
        args = NULL;
    }

    args = (args_t*)mem_alloc(sizeof(args_t));
    args->test_dir = NULL;
    args->record = false;
    args->test = false;
    
    for (int i = 1; i < argc; i++)
    {
        const char *arg = NULL;
        if (str_equals(argv[i], "--testdir"))
        {
            NEXTARG(arg);
            args->test_dir = arg;
        }
        else if (str_equals(argv[i], "--record=true"))
        {
            args->test = false;
            args->record = true;
        }
        else if (str_equals(argv[i], "--record=false"))
        {
            args->record = false;
        }
        else if (str_equals(argv[i], "--test=true"))
        {
            args->record = false;
            args->test = true;
        }
        else if (str_equals(argv[i], "--test=false"))
        {
            args->test = false;
        }
    }
}
