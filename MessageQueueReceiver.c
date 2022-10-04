//
// Group: Group D
// Author: Corey Hockersmith
// Email: cohocke@okstate.edu
// Date: 9/17/22
// Description: 
// To run, type "gcc -o MessageQueueReceiver MessageQueueReceiver.c -lrt" then "./MessageQueueReceiver

#include "MessageQueueReceiver.h"
#include "GetFileData.h"
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MQ_NAME "/queue_2"
char*** MessageQueueReceiver(char* queueName){

        struct mq_attr attributes = {
                .mq_flags=0,
                .mq_maxmsg=10,
                .mq_curmsgs=0,
                .mq_msgsize= 8128

        };
        fileStruct fileData;
        mqd_t queue = mq_open(queueName, O_RDONLY); //creating read only queue
        if((mq_receive(queue, (char *) &fileData, 9000000000, NULL))==-1){
            perror("mq_receive");
            exit(1);
        }
        char*** fileArray=calloc(fileData.fileRows, sizeof(char**));
        for(int row=0; row<fileData.fileRows; row++){
            fileArray[row]= calloc(fileData.fileColumns, sizeof(char*));//allocating column space
        }


//        printf("Number of Rows = %d\n", fileData.fileRows);
//        printf("Number of columns = %d\n", fileData.fileColumns);
        //printf("%lu", sizeof(fileData.fileArray));
        char buffer[512];
        for (int row = 0; row < fileData.fileRows; row++) {
            for (int column = 0; column < fileData.fileColumns; column++) {
                //printf("%-s", fileData.fileArray[row][column]);
                mq_receive(queue, (char *) &buffer, 9000000000, NULL);
                //printf("%-150s", buffer); //this sees line by line the file
                fileArray[row][column] = calloc(strlen(buffer) + 1, sizeof(char));
                strcpy(fileArray[row][column], buffer);
                //printf("%-150s", buffer); //this sees line by line the file


            }
            //printf("\n");
        }
        return fileArray;
}