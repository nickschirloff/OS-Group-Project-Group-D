//
// Group: Group D
// Author: Corey Hockersmith
// Email: cohocke@okstate.edu
// Date: 9/17/22
// Description: 
// To run: type "gcc -o MessageQueueSender MessageQueueSender.c GetFileData.c -lrt" then "./MessageQueueSender"

#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GetFileData.h"
#define MQ_NAME "/queue_2"

void MessageQueueSender(char* uniqueQueueName, fileStruct fileData){
    struct mq_attr attributes = {
            .mq_flags=0,
            .mq_maxmsg=10,
            .mq_curmsgs=0,
            .mq_msgsize= 8128//sizeof(fileStruct)

    };


    mqd_t queue = mq_open(uniqueQueueName, O_CREAT | O_WRONLY, 0660, &attributes); //creating write only queue

    mq_send(queue, (const char *) &fileData, 512, 1);
    char buffer[512];
    for (int row = 0; row < fileData.fileRows; row++) {
        for (int column = 0; column < fileData.fileColumns; column++) {
            strcpy(buffer, fileData.fileArray[row][column]);
            //printf("%-s", fileData.fileArray[row][column]);
            mq_send(queue, (const char *) &buffer, sizeof(buffer), 1);
        }
        //printf("\n");
    }
    //getchar();
    mq_close(queue);
    mq_unlink(uniqueQueueName);//destroyed when processes receiving close the queue
}