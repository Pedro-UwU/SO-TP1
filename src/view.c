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

    int total_files = 0;

    if (argc == 1) {
        char buff[BUFF_SIZE] = {0};
        if (read(0, buff, BUFF_SIZE) < 0) {
            exit_error("ERROR: Read Stdin view");
        }
        total_files = atoi(buff);
        printf("Buff: %s\n", buff);
        printf("Total files: %d\n", total_files);
    } // else if (argc == 2) {
    //     total_files = atoi(argv[1]);
    //     printf("Argv: %s", argv[1]);
    //     printf("Total files: %d\n", total_files);
    // } else {
    //     exit_error("ERROR: Too many arguments");
    // }

    // if (total_files < 0) {
    //     exit_error("ERROR: Number of files must be positive");
    // }



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
        //printf("Antes del sem\n");
        printf("Llego aca\n");
        if(sem_wait(sem) == ERROR) {
            exit_error("ERROR: Unable to acquire lock");
        }
        //printf("Pase el sem\n");
        if((shm - shm_backup) > 2 && *(shm-2) == END_CHAR ) {
            printf("Corto por ENDCHAR\n");
            break;
        }
        //printf("Pase el break\n");
        int current = printf("%d", strlen(shm));
        shm += (current);
        
    }
}

// char *open_shm(size_t shm_size) {
//     int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
//     if(shm_fd == ERROR ) {
//         exit_error("ERROR: Unable to open shared memory");
//     }
//     char *shm = (char *) mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
//     if(shm == MAP_FAILED) {
//         exit_error("ERROR: Unable to map shared memory");
//     }
//     close(shm_fd);
//     return shm;
// }

// sem_t *open_sem(const char *sem_name) {
//     sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
//     if(sem == SEM_FAILED) {
//         exit_error("ERROR: Unable to open semaphore");
//     }
// }

// void close_shm(const char *shm_name, size_t shm_size) {
//     //unmap shared memory
//     if (munmap((void *)shm_name, shm_size) == ERROR ){
//         exit_error("ERROR: Unable to unmap shared memory");
//     }
//     //unlink shared memory
//     if(shm_unlink(shm_name) == ERROR) {
//         exit_error("ERROR: Unable to unlink shared memory");
//     }
// }

// void close_sem(sem_t *sem_name) {
//     if(sem_close(sem_name) == ERROR) {
//         exit_error("ERROR: Unable to close semaphore");
//     }
// }
