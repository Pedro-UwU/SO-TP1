#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <error_handler.h>
#include <shared_utils.h>
#include <unistd.h>

void print_shm(sem_t *sem, char *shm, char *shm_backup);

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