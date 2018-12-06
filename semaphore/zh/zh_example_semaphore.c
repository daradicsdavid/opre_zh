#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void up(sem_id) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;

    if (semop(sem_id, &op, 1) < 0)
        perror("semop");
}

void down(sem_id) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;

    if (semop(sem_id, &op, 1) < 0)
        perror("semop");
}

void semaphoreExample() {
    key_t key = ftok("key_id", 1);
    int sem_id = semget(key, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (sem_id < 0) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    semctl(sem_id, 0, SETVAL, 1);

    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child > 0)    // parent
    {

        printf("Parent enters critical section\n");
        down(sem_id);
        printf("Parent entered critical section\n");
        sleep(3);
        printf("Parent exits critical section\n");
        up(sem_id);
        printf("Parent exited critical section\n");

        //Parent will wait for child to exit.
        wait(NULL);
    } else {// child
        printf("Child enters critical section\n");
        down(sem_id);
        printf("Child entered critical section\n");
        sleep(3);
        printf("Child exits critical section\n");
        up(sem_id);
        printf("Child exited critical section\n");
        exit(0);
    }

    //Destroy semaphore
    if (semctl(sem_id, 0, IPC_RMID) < 0)
        perror("semctl");
}