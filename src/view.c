#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/semaphore.h>
#include <error_handler.h>

#define ERROR -1
#define SHM_SIZE 4096
#define SHM_NAME "buffer"
#define SEM_NAME "semaphore"

/* based on "Operating Systems Concepts" (Silberschatz, Galvin, Gagne) page 134  */

/*
 * REVISAR:
 * Size de la shared memory queda predefinido o lo leemos como argumento?
 * Al crear la shared memory, parametros? create + r/w va bien?
 * print_shm?
 */

void print_shm(sem_t *sem, char *shm);
void close_sem(sem_t *sem);
void close_shm(const char *shm_name, size_t shm_size);
char *open_shm(size_t shm_size);
sem_t *open_sem(const char *sem_name);

int main(int argc, char *argv[]) {

    sem_t *sem;     // pointer to semaphore
    char *shm;      // pointer to shared memory object

    // open shared memory object and semaphore
    shm = open_shm(SHM_SIZE);
    sem = open_sem(SEM_NAME);

    // read and print data from shared memory
    print_shm(sem, shm);

    // close shared memory and semaphore
    close_sem(sem);
    close_shm(SHM_NAME, SHM_SIZE);
    return 0;
}

void print_shm(sem_t *sem, char *shm) {
    while(1) {
        if(sem_wait(sem) == ERROR) {
            exit_error("ERROR: Unable to acquire lock");
        }
        if(*shm == 0) {
            break;
        }
        int current = printf("%s", shm);
        shm += (current + 1) * sizeof(*shm);
    }
}

char *open_shm(size_t shm_size) {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if(shm_fd == ERROR ) {
        exit_error("ERROR: Unable to open shared memory");
    }
    char *shm = (char *) mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shm == MAP_FAILED) {
        exit_error("ERROR: Unable to map shared memory");
    }

    return shm;
}

sem_t *open_sem(const char *sem_name) {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if(sem == SEM_FAILED) {
        exit_error("ERROR: Unable to open semaphore");
    }
}

void close_shm(const char *shm_name, size_t shm_size) {
    //unmap shared memory
    if ( munmap((void *)shm_name, shm_size) == ERROR ){
        exit_error("ERROR: Unable to unmap shared memory");
    }
    //unlink shared memory
    if(shm_unlink(shm_name) == ERROR) {
        exit_error("ERROR: Unable to unlink shared memory");
    }
}

void close_sem(sem_t *sem_name) {
    if(sem_close(sem_name) == ERROR) {
        exit_error("ERROR: Unable to close semaphore");
    }
}
