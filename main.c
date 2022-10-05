////
//// Created by coreytv on 10/4/22.
////
//#include <stdio.h>
//#include <unistd.h>
//#include <sys/wait.h>
//#include "GetFileData.h"
//#include "MessageQueueSender.h"
//#include "MessageQueueReceiver.h"
//int main(){
//    pid_t pid1=fork();
//    if(pid1>0) {
//        MessageQueueSender("/test_10", GetFileData("bookInfo.txt", "Star rating"));
//        wait(0);
//    }
//    if(pid1==0) {
//        sleep(1);
//        printf("here\n");
//        char ***file = MessageQueueReceiver("/test_10");
//    }
//    return 0;
//}