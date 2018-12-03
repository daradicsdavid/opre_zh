#include <memory.h>
#include <stdio.h>
#include "example_struct.h"

void printStruct(struct Book book);

void printStructByPointer(struct Book *book);

void modifyStruct(struct Book *book);

void structExample() {
    struct Book book;
    struct Book book2;

    strcpy(book.title, "C Programming");
    strcpy(book.author, "Nuha Ali");
    strcpy(book.subject, "C Programming Tutorial");
    book.book_id = 6495407;
    printStruct(book);
    book2 = book;

    modifyStruct(&book);
    //Book2 will hold the original values of the book not the modified one.
    printStructByPointer(&book);
    printStructByPointer(&book2);


}

void printStruct(struct Book book) {
    printf("Printing book:\n");
    printf("Book title : %s\n", book.title);
    printf("Book author : %s\n", book.author);
    printf("Book subject : %s\n", book.subject);
    printf("Book book_id : %d\n", book.book_id);
}

void printStructByPointer(struct Book *book) {
    printf("Printing book by pointer:\n");
    printf("Book title : %s\n", book->title);
    printf("Book author : %s\n", book->author);
    printf("Book subject : %s\n", book->subject);
    printf("Book book_id : %d\n", book->book_id);
}


void modifyStruct(struct Book *book) {
    strcpy(book->title, "Not C Programming");
    strcpy(book->author, "Not Nuha Ali");
    strcpy(book->subject, "Not C Programming Tutorial");
    book->book_id = 2;
}
