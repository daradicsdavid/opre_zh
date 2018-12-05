#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sem.h>

#define STR_SIZE 256

typedef struct {
    long mtype;
    char mtext[STR_SIZE];
} msg_t;

void initrand()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void handler(int signo)
{
    printf("(Kevin) Eszrevettem, hogy a betoro probalkozik!\n");
}

void up(sem_id)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1;
    op.sem_flg = 0;

    if (semop(sem_id, &op, 1) < 0)
        perror("semop");
}

void down(sem_id)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1;
    op.sem_flg = 0;

    if (semop(sem_id, &op, 1) < 0)
        perror("semop");
}

int main(int argc, char **argv)
{
    key_t key = ftok(argv[0], 1);
    int mq = msgget(key, 0600 | IPC_CREAT);
    if (mq < 0)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    int shm_id = shmget(key, 5 * sizeof(int) + 5 * STR_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (shm_id < 0)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    int *shm_doors = (int *)shmat(shm_id, NULL, 0);
    int *shm_doors_orig = shm_doors;
    char *shm_res = (char *)(shm_doors + 5 * sizeof(int));
    char *shm_res_orig = shm_res;

    int sem_id = semget(key, 1, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (sem_id < 0)
    {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    semctl(sem_id, 0, SETVAL, 1);

    int pipe_k2b[2];		// kevin -> betörõ irányú csõ
    if (pipe(pipe_k2b) < 0)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int pipe_b2k[2];		// betörõ -> kevin irányú csõ
    if (pipe(pipe_b2k) < 0)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t child1 = fork();
    if (child1 < 0)
    {
        perror("fork #1");
        exit(EXIT_FAILURE);
    }

    if (child1 > 0)
    {
        pid_t child2 = fork();
        if (child2 < 0)
        {
            perror("fork #2");
            exit(EXIT_FAILURE);
        }

        if (child2 > 0) // parent
        {

            wait(NULL);
        }
        else // child2, betörõ
        {
            close(pipe_k2b[1]);
            close(pipe_b2k[0]);

            initrand();

            msg_t msg;
            if (msgrcv(mq, &msg, STR_SIZE, 1, 0) < 0)
                perror("msgrcv #1");

            printf("(Betoro) Hanyatt esek %s jatekon!\n", msg.mtext);

            printf("(Betoro) Megfenyegetem Kevint!\n");
            msg.mtype = 2;
            strcpy(msg.mtext, "csak varj, amig a kezunk koze nem kerulsz");
            if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
                perror("msgsnd #2");

            sleep(1);
            printf("(Betoro) Ujra probalkozom!\n");
            kill(child1, SIGUSR1);

            char buf[7];
            read(pipe_k2b[0], &buf, 7);

            int rnd = rand() % 100;
            printf("(Betoro) Ezzel dobott meg: %s, hatas: %d%%\n", buf, rnd + 1);

            char answer[STR_SIZE];
            if (rnd < 50)
                strcpy(answer, "na megallj csak");
            else
                strcpy(answer, "nem uszod meg szarazon");

            write(pipe_b2k[1], &answer, STR_SIZE);

            sleep(1);

            down(sem_id);
            printf("(Betoro) Ujra probalkozom!\n");
            int i;
            shm_doors = shm_doors_orig;
            for (i = 0; i < 5; i++)
            {
                if (*shm_doors == 1)
                    strcpy(shm_res, "jajj!");
                else
                    strcpy(shm_res, "ezt megusztam");

                shm_doors++;
                shm_res += STR_SIZE;
            }
            up(sem_id);

            close(pipe_k2b[0]);
            close(pipe_b2k[1]);
            exit(0);
        }

        wait(NULL);
    }
    else // child1, Kevin
    {
        close(pipe_k2b[0]);
        close(pipe_b2k[1]);

        sigset_t sigset;
        sigfillset(&sigset);
        sigprocmask(SIG_BLOCK, &sigset, NULL);

        initrand();
        int rnd = rand() % 31 + 20;

        printf("(Kevin) %d apro jatekot szorok szet!\n", rnd);

        msg_t msg;
        msg.mtype = 1;
        sprintf(msg.mtext, "%d", rnd);
        if (msgsnd(mq, &msg, STR_SIZE, 0) < 0)
            perror("msgsnd #1");

        if (msgrcv(mq, &msg, STR_SIZE, 2, 0) < 0)
            perror("msgrcv #2");

        printf("(Kevin) Betoro mondja: %s\n", msg.mtext);

        struct sigaction sigact;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_flags = 0;
        sigact.sa_handler = handler;
        sigaction(SIGUSR1, &sigact, NULL);

        sigdelset(&sigset, SIGUSR1);
        sigsuspend(&sigset);

        printf("(Kevin) Megdobalom festekkel!\n");
        write(pipe_k2b[1], "festek", 7);

        char buf[STR_SIZE];
        read(pipe_b2k[0], buf, STR_SIZE);
        printf("(Kevin) Betoro mondja: %s\n", buf);

        down(sem_id);
        printf("(Kevin) Kirakom a vodroket...\n");
        int i;
        for (i = 0; i < 5; i++)
        {
            *shm_doors = rand() % 2;
            shm_doors++;
        }
        printf("(Kevin) Kiraktam a vodroket\n");
        up(sem_id);

        close(pipe_k2b[1]);
        close(pipe_b2k[0]);
        exit(0);
    }

    printf("\n");

    int j;
    shm_res = shm_res_orig;
    shm_doors = shm_doors_orig;
    for (j = 0; j < 5; j++)
    {
        printf("(Szulo) Kevin rakott-e vodrot: %d, betoro mondta: %s\n", *shm_doors, shm_res);
        shm_doors++;
        shm_res += STR_SIZE;
    }

    if (msgctl(mq, IPC_RMID, NULL) < 0)
        perror("msgctl");

    if (shmctl(shm_id, IPC_RMID, NULL) < 0)
        perror("shmctl");

    if (semctl(sem_id, 0, IPC_RMID) < 0)
        perror("semctl");
}