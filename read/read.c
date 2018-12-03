#include <stdio.h>

void readString(char *line);

void readInt();

void readExample() {

    char line[50];
    readString(line);
    printf("Read line: %s \n", line);

    int number;
    readInt(&number);
    printf("Read number: %i \n", number);
}

void readString(char *line) {
    printf("Enter something:");
    scanf("%[^\n]", line);
}

void readInt(int *number) {
    printf("Enter a number:");
    scanf("%d", number);
}