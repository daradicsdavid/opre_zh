#include <stdio.h>

void printExample() {
    printf("This is an integer: %i \n", 1);
    printf("This is a float: %f \n", 1.111);
    char string[5] = {'h', 'e', 'l', 'l', 'o'};
    printf("This is a string: %s \n", &string);
}