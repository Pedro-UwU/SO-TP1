#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#define MAX_BUFF 1024

void toUpper(char * name);

void waitForNew();

int main(int argc, char const *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    int total_args = argc - 1;
    for (int i = 1; i <= total_args; i++) {
        toUpper((char*)argv[i]);
    }

    waitForNew();
}

void toUpper(char * name) {
    int i = 0;
    while (name[i]) {
        name[i] = toupper(*(name + i));
        i++;
    }
    printf("%s\n", name);
    char buff2[2048] = {0};
    sprintf(buff2, "ready_%s.txt", name);
    fopen(buff2, "w");
}

void waitForNew() {
    char buff[MAX_BUFF] = {0};
    //create_file("before_reading");
    int dim;
    while((dim = read(STDIN_FILENO, buff, MAX_BUFF)) > 0) {
        toUpper(buff);
        for (int j = 0; j < dim; j++) {
            buff[j] = 0;
        }
    }
    
}
