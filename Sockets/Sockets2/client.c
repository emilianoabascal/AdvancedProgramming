/*
    Program for a simple chat client
    The server address and port are provided as arguments to the program

    Gilberto Echeverria
    gilecheverria@yahoo.com
    26/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>

#include "sockets.h"

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024

void usage(char * program);
void connectToServer(char * address, char * port);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    printf("\n=== CLIENT PROGRAM ===\n");

    if (argc != 3)
        usage(argv[0]);

    connectToServer(argv[1], argv[2]);

    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Establish a connection with the server indicated by the parameters
void connectToServer(char * address, char * port)
{
    struct addrinfo hints;
    struct addrinfo * server_info;
    int connection_fd;

    // Prepare the information to determine the local address
    bzero(&hints, sizeof hints);
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    ///// GETADDRINFO
    // Get the actual address of this computer
    if (getaddrinfo(address, port, &hints, &server_info) == -1)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    ///// SOCKET
    // Use the information obtained by getaddrinfo
    connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (connection_fd == -1)    
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    ///// CONNECT
    // Connect with the desired port
    if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    // Establish the communication
    communicationLoop(connection_fd);

    // Close the socket
    close(connection_fd);
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    int chars_read;
    int max_val;
    int guess;
    
    // Handshake
    sprintf(buffer, "HELLO");
    sendMessage(connection_fd, buffer, strlen(buffer));
    receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    if (strncmp(buffer, "RANGE", 6) == 0) 
    {
        printf("Enter max value to guess: ");
        scanf("%d", &max_val);
    }
    
    sprintf(buffer, "%d", max_val);
    sendMessage(connection_fd, buffer, strlen(buffer));
    receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    
    // Start game
    while ( strncmp(buffer, "CORRECT", 8) != 0)
    {
        printf("Enter your guess: ");
        scanf("%d", &guess);
        sprintf(buffer, "%d", guess);
        sendMessage(connection_fd, buffer, strlen(buffer));
    
        chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
        // Finish the loop if the server disconnected
        if (!chars_read)
            break;
        if ( strncmp(buffer, "CORRECT", 8) != 0)
        {
            printf("Choose a number that is %s: ", buffer);
        }
        else
        {
            printf("You guessed correctly!\n");
        }
    }
    
    sprintf(buffer, "BYE");
    sendMessage(connection_fd, buffer, strlen(buffer));
    chars_read = receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    
}