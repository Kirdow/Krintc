#ifndef _ARGS_H_
#define _ARGS_H_

#include <stdbool.h>

typedef struct {
    const char *test_dir;
    bool record;
    bool test;
} args_t;

void args_load(int argc, const char *argv[]);
const args_t *args_get();
void args_free();

#endif
