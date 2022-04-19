#ifndef MAIN_H
#define MAIN_H
#define MAX_SLAVES 2
#define OUTPUT_FILE "output.txt"
#define FD_WRITE 1
#define FD_READ 0
#define STDIN 0
#define STDOUT 1
#define INITIAL_FILES 2
#define SLAVE_COMMAND "./dummy_slave"

#include <semaphore.h>


typedef struct master_conf {
    int total_jobs;
    int assigned_jobs;
    int done_jobs;
} master_conf;

typedef struct slave {
    int pid;
    int stdin_fd;
    int stdout_fd;
    int done_jobs;
} slave;

void init_slaves(slave * slaves, int total_slaves, char * files[], master_conf * conf);
void start_executing(slave slaves[], int total_slaves, char * buffer, int * buffer_index, char * files[], sem_t * sem, master_conf * conf);
void close_fds(slave slaves[], int dim);
void kill_slaves(int dim);

#endif