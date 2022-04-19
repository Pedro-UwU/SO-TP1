// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <error_handler.h>
#include <stdio.h>

void exit_error(char * msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}