#include <error_handler.h>
#include <stdio.h>

void exit_error(char * msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}