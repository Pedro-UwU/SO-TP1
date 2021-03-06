#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include <stddef.h>
#include <semaphore.h>

#define SHM_SIZE 65536
#define SHM_NAME "shared_buffer"
#define SEM_NAME "la_pizza_de_don_cangrejo"
#define END_CHAR '~'

void *open_shm(size_t shm_size);
sem_t *open_sem(const char *sem_name);
void close_shm(void *shm_addr, char * name, size_t shm_size);
void close_sem(sem_t *sem);


#endif