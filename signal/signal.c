#include <zconf.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


void signalFromChild(int pid) {
    signal(SIGUSR2, signalFromChild);
    printf("Signal from child! \n");
    exit(0);
}

void signalExample() {
    int parentId = getpid();
    int pid = fork();

    if (pid == 0) {

        kill(parentId, SIGUSR2);
        sleep(3);
    } else {
        signal(SIGUSR2, signalFromChild);
        sleep(10);
    }
}