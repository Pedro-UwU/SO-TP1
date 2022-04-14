#ifndef MAIN_H
#define MAIN_H
#define MAX_SLAVES 6
#define OUTPUT_FILE "output.txt"
#define FD_WRITE 1
#define FD_READ 0
#define STDIN 0
#define STDOUT 1
#define INITIAL_FILES 2
#define SLAVE_COMMAND "./dummy_slave"

typedef struct master_conf {
    int total_jobs;
    int assigned_jobs;
    int done_jobs;
} master_conf;

typedef struct slave {
    int pid;
    int stdin_fd;
    int stdout_fd;
} slave;

void init_slaves(slave * slaves, int total_slaves, char * files[], master_conf * conf);




#endif