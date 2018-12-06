#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#define STR_SIZE 256

typedef struct {
    long mtype;
    char mtext[STR_SIZE];
    int mint;
} msg_t;

int send_message_on_queue(int mq, msg_t *msg) {
    int result, length;

    /* The length is essentially the size of the structure minus sizeof(mtype) */
    length = sizeof(msg_t) - sizeof(long);

    if ((result = msgsnd(mq, msg, length, 0)) == -1) {
        return (-1);
    }

    return (result);
}

int read_message_on_queue(int qid, long type, msg_t *msg) {
    int result, length;

    /* The length is essentially the size of the structure minus sizeof(mtype) */
    length = sizeof(msg_t) - sizeof(long);

    if ((result = msgrcv(qid, msg, length, type, 0)) == -1) {
        return (-1);
    }

    return (result);
}

void messageQueueExampe() {

    //We need to generate an identifier key
    key_t key = ftok("identifier_string", 1);

    //Gets the message queue identified by the key, or creates one if not exists yet.
    int mq = msgget(key, 0600 | IPC_CREAT);

    //Message queue creation error handling
    if (mq < 0) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }


    /* <SEND MESSAGE> */
    //Declare message
    long type = 1;
    msg_t msg;
    //Add type
    msg.mtype = type;

    //Write the third param formatted by the second param into first param: sprintf(string,"%d",1); => string == "1"
    sprintf(msg.mtext, "%s", "Szöveg");
    msg.mint = 4;

    send_message_on_queue(mq, &msg);

    msg_t read_msg;
    read_message_on_queue(mq, type, &read_msg);
    printf("%s - %i", read_msg.mtext, read_msg.mint);
    /* </SEND MESSAGE> */


}

