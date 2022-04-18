#include <shared_utils.h>
#include <error_handler.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>

#define S_RWALL S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH

#define INITIAL_VALUE 1

void *open_shm(size_t shm_size) {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_RWALL);
    if(shm_fd == ERROR ) {
        exit_error("ERROR: Unable to open shared memory");
    }
    void *shm = mmap(NULL, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if(shm == MAP_FAILED) {
        exit_error("ERROR: Unable to map shared memory");
    }

    if (ftruncate(shm_fd, shm_size) < 0) {
        exit_error("ERROR: ftruncate");
    }

    if (close(shm_fd) == ERROR) {
        exit_error("EEROR: close shm_fd");
    }
    return shm;
}

sem_t *open_sem(const char *sem_name) {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_RDWR, S_RWALL, INITIAL_VALUE);
    if(sem == SEM_FAILED) {
        exit_error("ERROR: Unable to open semaphore");
    }
    return sem;
}

void close_shm(void *shm_addr, char * name, size_t shm_size) {
    //unmap shared memory
    if (munmap(shm_addr, shm_size) == ERROR ){
        exit_error("ERROR: Unable to unmap shared memory");
    }
    //unlink shared memory
    if(shm_unlink(name) == ERROR) {
        exit_error("ERROR: Unable to unlink shared memory");
    }
}

void close_sem(sem_t *sem) {
    if(sem_close(sem) == ERROR) {
        exit_error("ERROR: Unable to close semaphore");
    }
}