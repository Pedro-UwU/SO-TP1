#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_BUFF 1024

void create_file(const char * name);

void waitForNew();

int main(int argc, char const *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Child Born\n");
    fflush(stdout);
    waitForNew();
    return 0;

}

void create_file(const char * name) {
    FILE * aux = fopen(name, "w");
    fclose(aux);
    
}

void waitForNew() {
    char buff[MAX_BUFF];
    int count = 1000;
    //create_file("before_reading");
    int dim;
    while((dim = read(STDIN_FILENO, buff, MAX_BUFF)) > 0) {
        printf("ASASDSDADASDASD\n");
        sleep(1);
    }
}
