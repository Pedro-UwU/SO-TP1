#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/semaphore.h>

#define ERROR -1

/* based on [DINOSAUR BOOK] page 134 figure 3.17 */

void printShm(sem_t *sem, char *shm);

int main(int argc, char *argv[]) {

    /* the size (in bytes) of shared memory object  -> leer o elegir valor directamente? */
    const int SIZE = 4096;

    /* name of the shared memory object */
    const char *name = "buffer";

    /* shared memory file descriptor */
    int fd;

    /* pointer to shared memory obect */
    char *shm;

    /* pointer to semaphore*/
    sem_t *sem;

    /* open the shared memory object -> create + r/w ta bien? */
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(fd == ERROR ) {
        perror("ERROR: Unable to open shared memory");
    }

    /* memory map the shared memory object */
    shm = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shm == MAP_FAILED) {
        perror("ERROR: Unable to map shared memory");
    }

    sem = sem_open("semaphore", O_CREAT, 0644, 1);
    if(sem == SEM_FAILED) {
        perror("ERROR: Unable to open semaphore");
    }

    // read and print data from buffer
    printShm(sem, shm);

    // close shared memory and semaphore
    sem_close(sem);

    /* remove the shared memory object */
    shm_unlink(name); return 0;
}

void printShm(sem_t *sem, char *shm) {
    while(1) {
        wait(sem);
        if(*shm == 0) {
            break;
        }
        int current = printf("%s", shm);
        shm += (current + 1) * sizeof(*shm);
    }
}