/*
*   Group D
*   Nick Schirloff
*   nick.schirloff@okstate.edu
*   10/2/22
*   This is the client program that will interact with the sever. It reads in the user's
*   input, and sends it to the server, but stores no data on its side. It first checks that
*   the number of active clients is not exceeding the limit (3), and it so, it terminates.
*   Afterwards, the client gets the relevant file name and column name from the user, and
*   prints the options to perform on the column. Then, it loops until receiving input
*   that designates it to exit, sending the relevant command(s) to the server
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Socket imports
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
//#include "GetFileData.h"
// Define the port and IP for easy use
#define PORT 5002
#define IP "127.0.0.1"

int main() {

    int client_socket, return_socket;
    struct sockaddr_in server_address;
    char buffer[1024];
    char temp;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0) {
        printf("Client failed to establish socket.\n");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    return_socket = connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if(return_socket < 0) {
        printf("Client failed to connect to server.\n");
        return -1;
    }

    // Important sidenote:
    // Make sure to include \n in prints for server/client messages to
    // print correctly.
    // Also make sure to correctly align sends and reads, as the server
    // or client will hang otherwise

    printf("\nSuccessfully connected to server.\n\n");

    read(client_socket, buffer, sizeof(buffer));
    if(strcmp(buffer, "-1\n") == 0) {
        printf("Server has too many connections already. Please try again later.\n");
        return -1;
    } else { // Successfully connected
        // "Please choose file" line
        read(client_socket, buffer, sizeof(buffer));
        printf("%s\n", buffer);

        // Reading and printing file names
        read(client_socket, buffer, sizeof(buffer));
        printf("%s\n", buffer);

        // Read in user's chosen file, send to server
        scanf("%s", buffer);
        send(client_socket, buffer, sizeof(buffer), 0);

        // Read columns from sent file
        printf("\nAvailable options for %s: \n", buffer);
        read(client_socket, buffer, sizeof(buffer));
        printf("%s", buffer);

        // Read in user's column selection
        scanf("%c", &temp);
        fgets(buffer, 30, stdin);
        send(client_socket, buffer, sizeof(buffer), 0);

        // Get the options to perform on records from server
        char file_options[254];
        read(client_socket, file_options, sizeof(file_options));

        // Create large char array to hold the contents of amazonBestsellers
        char msg_buffer[10240];

        // Create int to maintain while loop
        int lock = 0;
        // Enter main loop
        while(lock != 4)
        {
            printf("Buff: %s\n", buffer);
            printf("Size buff: %lu\n", sizeof(buffer));
            printf("Size num: %lu\n", sizeof("4"));
            printf("Buff num: %i\n", atoi(buffer));
            //printf("Buff[0]: %c\n", &buffer[0]);
            // Print the options that can be performed on the records
            printf("%s", file_options);

            // Reading user's selected operation to perform, and send it to the server
            scanf("%s", buffer);
            send(client_socket, buffer, sizeof(buffer), 0);
            lock = atoi(buffer);
            printf("Lock: %i\n", lock);

            // Read in server's response based on user's operation
            read(client_socket, buffer, sizeof(buffer));
      
            //Handle incorrect input
            while(strcmp(buffer, "-1") == 0) {
                printf("Invalid operation. Please enter a new value.\n");
                scanf("%s",buffer);
                send(client_socket, buffer, sizeof(buffer), 0);
                read(client_socket, buffer, sizeof(buffer));
            }
            // Otherwise, operate off of user selected option

            switch(atoi(buffer)) {
                case 1:
                    // Read message from server (Please choose which record to display)
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    printf("%s", buffer);
                    // Read in the available records, print
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    printf("%s", msg_buffer);
                    // Read user choice of record, send
                    scanf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer),0);

                    break;
                case 2:
                    // Read message from server (Please choose which record to save)
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    printf("%s", buffer);
                    // Read in available records, print
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    printf("%s", msg_buffer);
                    // Read user's choice of record, send
                    scanf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer),0);

                    break;
                case 3: 
                    //display here
                    break;
                case 4:
                    // Read goodbye message, print, quit
                    read(client_socket, buffer, sizeof(buffer));
                    printf("%s", buffer);
                    lock = 1;
                    break;
                default: break;

            }

        } // End of main loop
        



        // printf("%s",file_options);


        // // Reading the user's selected operation to perform
        // scanf("%s", buffer);
        // send(client_socket, buffer, sizeof(buffer), 0);
        // //int user_in;
        // while(strcmp(buffer, "4") != 0) {
        //     // Read server's response based on user's operation
        //     read(client_socket, buffer, sizeof(buffer));
        //     // Handle incorrect input
        //     while(strcmp(buffer, "-1") == 0) {
        //         printf("Invalid operation. Please enter a new value.\n");
        //         scanf("%s",buffer);
        //         send(client_socket, buffer, sizeof(buffer), 0);
        //         read(client_socket, buffer, sizeof(buffer));
        //     }
            
        //     // Operate based off of user selected option
        //     switch(atoi(buffer)) {
        //         case 1:
        //             // Read message from server (Please choose which record to display)
        //             read(client_socket, msg_buffer, sizeof(msg_buffer));
        //             printf("%s", buffer);
        //             // Read in the available records, print
        //             read(client_socket, msg_buffer, sizeof(msg_buffer));
        //             printf("%s", msg_buffer);
        //             // Read user choice of record, send
        //             scanf("%s",buffer);
        //             send(client_socket, buffer, sizeof(buffer),0);

        //             break;
        //         case 2:
        //             read(client_socket, msg_buffer, sizeof(msg_buffer));
        //             //printf("%s", buffer);
        //             read(client_socket, msg_buffer, sizeof(msg_buffer));
        //             printf("%s", msg_buffer);
        //             scanf("%s",buffer);
        //             send(client_socket, buffer, sizeof(buffer),0);
        //             break;
        //         case 3: 
        //             //display here
        //             break;
        //         case 4:
        //             break;
        //         default: break;
        //     }
        //     //send(client_socket, buffer, sizeof(buffer), 0);
        // }
        printf("Exiting...\n");
    }

    close(client_socket);


    return 0;
}
