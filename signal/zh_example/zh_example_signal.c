#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int sig_value;

void handler(int signo, siginfo_t *info, void *ptr) {
    sig_value = info->si_value.sival_int;
    printf("%d\n", sig_value);
}

void sendSignal() {
    union sigval value;
    value.sival_int = 3;
    sigqueue(getppid(), SIGUSR1, value);
}

void setupReceiveSignal() {
    struct sigaction sigact;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_SIGINFO;
    sigact.sa_sigaction = handler;
    sigaction(SIGUSR1, &sigact, NULL);

    sigset_t sigset;
    sigfillset(&sigset);
    sigdelset(&sigset, SIGUSR1);
    sigsuspend(&sigset);
}

void signalExample() {
    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child > 0)    // parent
    {
        setupReceiveSignal();
    } else {// child
        sendSignal();
    }

}