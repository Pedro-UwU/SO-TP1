#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <error_handler.h>
#include <shared_utils.h>
#include <unistd.h>

#define BUFF_SIZE 4

// #define ERROR -1
// #define SHM_SIZE 4096
// #define SHM_NAME "buffer"
// #define SEM_NAME "semaphore"

/* based on "Operating Systems Concepts" (Silberschatz, Galvin, Gagne) page 134  */

/*
 * REVISAR:
 * Size de la shared memory queda predefinido o lo leemos como argumento?
 * Al crear la shared memory, parametros? create + r/w va bien?
 * print_shm?
 */

void print_shm(sem_t *sem, char *shm, char *shm_backup);
// void close_sem(sem_t *sem);
// void close_shm(const char *shm_name, size_t shm_size);
// char *open_shm(size_t shm_size);
// sem_t *open_sem(const char *sem_name);

int main(int argc, char *argv[]) {

    sem_t *sem;     // pointer to semaphore
    char *shm, *shm_backup;      // pointer to shared memory object
    setvbuf(stdout, NULL, _IONBF, 0);
    // open shared memory object and semaphore
    shm = open_shm(SHM_SIZE);
    sem = open_sem(SEM_NAME);
    shm_backup = shm;
    // read and print data from shared memory


    print_shm(sem, shm, shm_backup);

    // close shared memory and semaphore
    close_sem(sem);
    close_shm(shm_backup, SHM_NAME, SHM_SIZE);
    return 0;
}

void print_shm(sem_t *sem, char *shm, char *shm_backup) {
    while(1) {
        if(sem_wait(sem) == ERROR) {
            exit_error("ERROR: Unable to acquire lock");
        }
        if((shm - shm_backup) > 1 && *(shm-1) == END_CHAR ) {
            break;
        }
        int current = printf("%s", shm);
        shm += (current);
        
    }
}