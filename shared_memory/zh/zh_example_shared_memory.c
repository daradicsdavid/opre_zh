#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

void writeToSharedMemory(int *shm) {
    *shm = 3;
    shm++;
    *shm = 2;
    shm++;
    *shm = 1;
}

void readSharedMemory(int *shm) {
    int j;
    for (j = 0; j < 3; j++) {
        printf("%i - %i \n", j, *shm);
        shm++;
    }
}


void sharedMemoryExample() {
    key_t key = ftok("shared_memory_id", 1);

    //Creates shared memory
    int shm_id = shmget(key, 3 * sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    //Attaches shared memory
    int *shm = (int *) shmat(shm_id, NULL, 0);
    //Story the beginning of the shared memory
    int *shm_orig = shm;

    writeToSharedMemory(shm);
    shm = shm_orig;
    readSharedMemory(shm);

    if (shmctl(shm_id, IPC_RMID, NULL) < 0)
        perror("shmctl");
}



