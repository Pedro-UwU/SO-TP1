#ifndef ERROR_HANDLER
#define ERROR_HANDLER
#include <stdio.h>
#include <stdlib.h>

void exit_error(char * msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

#endif