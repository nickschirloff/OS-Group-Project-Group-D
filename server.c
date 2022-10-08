#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Socket imports
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
//#include "GetFileData.h"
//#include "forking.h"
//#include "forked_process_piping_test.h"

#define PORT 5002
#define IP "127.0.0.1"
#define MAX_CLIENTS 3

int main() {

    int server_socket, return_socket;
    int bind_return;
    char buffer[1024];
    pid_t child;
    int status;

    struct sockaddr_in server_address;
    struct sockaddr_in return_address;
    socklen_t address_size;

    if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Failed to establish server socket.\n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind_return = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(bind_return < 0)
    {
        printf("Failed to bind server.\n");
        return -1;
    }

    if((listen(server_socket, MAX_CLIENTS)) < 0)
    {
        printf("Failed to open server to listen.\n");
        return -1;
    }

    printf("Server open. Listening for clients....\n");
    // total_connections can be used to give unique id to each process
    // active_connections maintains the number of active clients, never going over three
    int active_connections = 0;
    // Main loop
    while(1) {

        return_socket = accept(server_socket, (struct sockaddr*)&return_address, &address_size);
        if(return_socket < 0) {
            //printf("Failed to connect client to server.\n");
            exit(-1);
        }

        // Handle number of clients breaching number of clients
        // Send "-1" if so, client will terminate afterwards
        if((active_connections+1) > MAX_CLIENTS) {
            send(return_socket, "-1\n", sizeof("-1\n"), 0);
        } else {
            
            send(return_socket, "1\n", sizeof("1\n"), 0);

            active_connections++;
            printf("Client connected. Current active connections: %i\n", active_connections);
            if((child = fork()) == 0) {
                close(server_socket);
                char welcome_message[] = "Hello, client. Please choose a file:\n";
                send(return_socket, welcome_message, sizeof(welcome_message), 0);

                FILE *file_pointer;
                // Opening options.txt
                if((file_pointer = fopen("options.txt", "r")) == NULL) {
                    printf("Failed to open options.txt.\n");
                }

                // Init buffer to hold file data, arbitrary size of 254
                char file_buffer[254];
                // Read first line into the file buffer, parse based around the comma delimiter
                fscanf(file_pointer, "%s", file_buffer);
                char *file_1 = strtok(file_buffer, ",");
                char *file_2 = strtok(NULL, " ");
                char *current_filename;

                // Debug prints
                //printf("F1: %s\n", file_1);
                //printf("F2: %s\n", file_2);

                // Maybe add a while loop here to handle no file found
                if(file_1 == NULL && file_2 == NULL) {              // If no files are found
                    send(return_socket, "null", sizeof("null"), 0);
                } else if(file_2 == NULL) {                         // If there is only one file
                    send(return_socket, file_1, sizeof(file_1), 0);
                } else {                                            // If both files are present
                    char temp[254];
                    strcpy(temp, file_1);
                    strcat(temp, ", ");
                    strcat(temp, file_2);
                    // Debug
                    //printf("Sending: %s\n", temp);
                    send(return_socket, temp, sizeof(temp), 0);
                }

                // Read in client's selected file, check to make sure its valid
                // Possibly need another while loop here
                read(return_socket, buffer, sizeof(buffer));

                // Debug
                //printf("Selected file: %s\n", buffer);

                if(strcmp(buffer, file_1) == 0) {
                    printf("File chosen: bookInfo.txt\n");
                    current_filename = file_1;
                } else if(strcmp(buffer, file_2) == 0) {
                    printf("File chosen: amazonBestsellers.txt\n");
                    current_filename = file_2;
                } else {
                    printf("Failed to find client's file.\n");
                }

                // Create buffer to hold data that will be stored into
                // the actual string that is sent to client
                char column_buffer[254];
                char full_column_string[254];
                // Copy first line so we can copy string
                fgets(column_buffer, sizeof(column_buffer), file_pointer);
                strcpy(full_column_string, column_buffer);
                while(fgets(column_buffer, sizeof(column_buffer), file_pointer) != NULL)
                {
                    strcat(full_column_string, column_buffer);
                }
                // Send the column options to client
                send(return_socket, full_column_string, sizeof(full_column_string), 0);

                // Getting user's column selection
                read(return_socket, buffer, sizeof(buffer));
                char *selected_column = buffer;

                // Commented out for the moment, as my MAC can't run these files lol

//                printf("%s\n", selected_column);
//
//                //GetFileData("current_filename", selected_column);
//                printf("%lu %lu", strlen(current_filename), strlen(selected_column));
//                printf("%s %s", selected_column, current_filename);

                // fileStruct test = GetFileData(current_filename, selected_column);
                // pipeStruct pipeData;
                // pipeData.data_pipe=calloc(test.numberOfUniques, sizeof(int*));
                // pipeData.what_pipe=calloc(test.numberOfUniques, sizeof(int*));
                // for(int i=0;i<test.numberOfUniques;i++) {

                //     pipeData.data_pipe[i]=calloc(2, sizeof(int));
                //     pipeData.what_pipe[i]=calloc(2, sizeof(int));
                //     if(pipe(pipeData.data_pipe[i]) < 0){
                //         exit(1);
                //     }
                //     if(pipe(pipeData.what_pipe[i]) < 0){
                //         exit(1);
                //     }
                // }
                // printf("Num cols: %i\n", test.fileColumns);
                // Pass current_filename and selected_column to Corey
                // struct = GetFileData(current_filename, selected_column)
//                // fork
//                 pid_t pid1=fork();
//                 if(pid1==0) {
//                     forking(test, pipeData);
// //                    forked_process_piping_test(test.targetColumnIndex, test.uniqueArray[0], pipeData.what_pipe[0], pipeData.data_pipe[0],
// //                                               test.fileRows, test.fileColumns,
// //                                               test.fileArray);
//                     printf("done forking\n");
//                     break;
//                 }
                //sleep(5);
                //forking(test, pipeData);

                //printf("Num cols: %i\n", 15);
                //gcc server.c GetFileData.c MessageQueueSender.c MessageQueueReceiver.c forking.c forked_process_piping_test.c -o ./main

                // String to hold options to perform. Send them to the client
                char options[] = "1. Display Records\n2. Save Records\n3. Display Summary\n4. Exit\n";
                send(return_socket, options, sizeof(options), 0);

                int user_option = 0;
                // Init large array to hold the entire amazonBestsellers.txt file
                char msg[10240];
                while(user_option != 4)
                {
                    // Read the option selected by user
                    read(return_socket, buffer, sizeof(buffer));
                    user_option = atoi(buffer);
                    printf("Got: %i\n", user_option);
                    
                    // Switch statement based off of user option
                    switch(user_option) {
                        case 1:
                            // Create record display message, send to client
                            strcpy(msg, "Please choose which record to display:\n");
                            //printf("Msg: %s", msg);
                            send(return_socket, msg, sizeof(msg), 0);
                            
                            // Create string of acquired uniques from message queue
                            // Commented out due to my environment
                            // Temporary message v Replace w/ for loop
                            strcpy(msg, "This where we get uniques. Case 1\n");
                            // for(int uniques=0; uniques<test.numberOfUniques; uniques++){
                            //     char tempMsg[256];
                            //     sprintf(tempMsg, "%d) %s\n",uniques, test.uniqueArray[uniques]);
                            //     strcat(msg, tempMsg);
                            // }

                            // Send the string of available records to the user
                            send(return_socket, msg, sizeof(msg), 0);
                            // Read the user's selected record
                            read(return_socket, buffer, sizeof(buffer));
                            //printf("%s\n", test.uniqueArray[atoi(buffer)]);

                            // pipes
                            //
                            // char pipeBuffer[10240];
                            // char what[80];
                            // sprintf(what, "display");
                            // int ndata;
                            // ndata = write(pipeData.what_pipe[atoi(buffer)][1], what, 80);
                            // printf("here1 %d\n", ndata);
                            // printf("Buffer is\n %s\n", pipeBuffer);
                            // ndata = read(pipeData.data_pipe[atoi(buffer)][0], pipeBuffer, 10240);
                            // printf("here2 %d\n", ndata);
                            // printf("Buffer is\n %s\n", pipeBuffer);
                            // wait(0);

                            break;
                        case 2:
                            // Create record save message, send to client
                            strcpy(msg, "Please choose which record to save:\n");
                            //printf("Msg: %s", msg);
                            send(return_socket, msg, sizeof(msg), 0);

                            // Create string of acquired uniques from message queue
                            // Commented out due to my environment
                            // Temporary message v Replace w/ for loop
                            strcpy(msg, "This where we get uniques. Case 1\n");
                            // for(int uniques=0; uniques<test.numberOfUniques; uniques++){
                            //     char tempMsg[256];
                            //     sprintf(tempMsg, "%d) %s\n",uniques, test.uniqueArray[uniques]);
                            //     strcat(msg, tempMsg);
                            // }

                            // Send the string of available records to the user
                            send(return_socket, msg, sizeof(msg), 0);
                            // Read the user's selected record
                            read(return_socket, buffer, sizeof(buffer));
                            //printf("%s\n", test.uniqueArray[atoi(buffer)]);
                            // pipe here?
                            break;
                        case 3:

                            break;
                        case 4:
                            printf("Client disconnecting...\n");
                            // Creating 'goodbye client' message
                            strcpy(msg, "Goodbye, client.\n");
                            send(return_socket, msg, sizeof(msg), 0);
                            active_connections--;
                            // Loop breaks afterwards. Since user_option is initialized to 0,
                            // this part of the switch statement will not be reached too early
                            break;
                    } // End of file options switch
                } // End of file options loop

            } else {
                wait(0);
            } // fork() if statement
        } // Main if statement

    } // End of main loop

    return 0;
}