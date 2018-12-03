#include <zconf.h>
#include <stdio.h>
#include <stdlib.h>

void startChild(int parentToChild[2], int childToParent[2]);

void startParent(int parentToChild[2], int childToParent[2]);

void sendIntArray(int *array, int arraySize, int *pipe);

int receiveIntArray(int *array, int *arraySize, int *pipe);


int setupPipes(int parentToChild[], int childToParent[]) {
    if (pipe(parentToChild) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 0;
    }
    if (pipe(childToParent) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 0;
    }
    return 1;
}

void pipeExample() {
    printf("Starting pipe example \n");
    int parentProcessId = getpid();

    int parentToChild[2];
    int childToParent[2];

    if (!setupPipes(parentToChild, childToParent)) {
        exit(1);
    }

    int pid = fork();

    if (pid == 0) {
        startChild(parentToChild, childToParent);
    } else {
        startParent(parentToChild, childToParent);
    }

}

void startChild(int parentToChild[2], int childToParent[2]) {
    int array[40];
    int arraySize = 0;
    receiveIntArray(array, &arraySize, parentToChild);
    printf("Client received:\n");
    for (int i = 0; i < arraySize; ++i) {
        printf("%i \n", array[i]);
    }
    sendIntArray(array, arraySize, childToParent);
}

void startParent(int parentToChild[2], int childToParent[2]) {

    int array[3] = {1, 2, 3};
    sendIntArray(array, 3, parentToChild);
    int receiveArray[40];
    int arraySize = 0;
    receiveIntArray(receiveArray, &arraySize, childToParent);
    printf("Server received:\n");
    for (int i = 0; i < arraySize; ++i) {
        printf("%i \n", receiveArray[i]);
    }
}

void sendIntArray(int *array, int arraySize, int *pipe) {
    close(pipe[0]);
    // Write concatenated string and close writing end
    write(pipe[1], &arraySize, sizeof(int));
    write(pipe[1], array, arraySize * sizeof(int));
}

int receiveIntArray(int *array, int *arraySize, int *pipe) {
    close(pipe[1]);
    ssize_t result = read(pipe[0], arraySize, sizeof(int));
    if (result == 0) {
        return 0;
    }
    read(pipe[0], array, (*arraySize) * sizeof(int));
    return 1;
}
