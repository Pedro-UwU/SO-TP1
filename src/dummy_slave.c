#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <separator.h>
#define MAX_BUFF 65536
#define COMMAND_BUFF 1024
#define EXEC_CMD "minisat"             //Command that will be executed
#define GREP_PARSE_CMD "grep -o -e \"Number of .*[0-9]\\+\" -e \"CPU time.*\" -e \".*SATISFIABLE\"" //Parser


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

    char command[COMMAND_BUFF];
    sprintf(command, "%s %s | %s", EXEC_CMD, name, GREP_PARSE_CMD);

    FILE * output = popen(command, "r");
    if (output == NULL) {
        fprintf(stderr, "ERROR: popen\n");
        exit(1);
    }

    char output_str[MAX_BUFF];
    fread(output_str, sizeof(char), MAX_BUFF, output);
    printf("Filename: %s\nPID: %d\n%s%s", name, getpid(), output_str, SEPARATOR);
}

void waitForNew() {
    char buff[MAX_BUFF] = {0};
    //create_file("before_reading");
    int dim;
    while((dim = read(STDIN_FILENO, buff, MAX_BUFF)) > 0) {
        toUpper(buff);

        //Clean buffer
        for (int j = 0; j < dim; j++) {
            buff[j] = 0;
        }
        sleep(1);
    }
    
}
