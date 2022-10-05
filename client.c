#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Socket imports
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include "GetFileData.h"
// Define the port and IP for easy use
#define PORT 5000
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
    } else {
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
        printf("%s",file_options);

        // Temp buffer so the real buffer doesn't get values it shouldn't
        char temp_buffer[254];
        char msg_buffer[10240];

        // Reading the user's selected operation to perform
        scanf("%s", buffer);
        send(client_socket, buffer, sizeof(buffer), 0);
        int user_in;
        while(strcmp(buffer, "4") != 0) {
            // Read server's response based on user's operation
            read(client_socket, buffer, sizeof(buffer));
            // Handle incorrect input
            while(strcmp(buffer, "-1") == 0) {
                printf("Invalid operation. Please enter a new value.\n");
                scanf("%s",buffer);
                send(client_socket, buffer, sizeof(temp_buffer), 0);
                read(client_socket, buffer, sizeof(buffer));
            }
            
            // Operate based off of user selected option
            switch(atoi(buffer)) {
                case 1:
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    //printf("%s", buffer);
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    printf("%s", msg_buffer);
                    scanf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer),0);
                    // display based off of chosen record

                    break;
                case 2:
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    //printf("%s", buffer);
                    read(client_socket, msg_buffer, sizeof(msg_buffer));
                    printf("%s", msg_buffer);
                    scanf("%s",buffer);
                    send(client_socket, buffer, sizeof(buffer),0);
                    break;
                case 3: 
                    //display here
                    break;
                default: break;
            }
            //send(client_socket, buffer, sizeof(buffer), 0);
        }
        printf("Exiting...\n");
    }

    close(client_socket);


    return 0;
}
