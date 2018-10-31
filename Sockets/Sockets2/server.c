/*
    Program for a simple chat server
    Can only connect with one client at a time

    Gilberto Echeverria
    gilecheverria@yahoo.com
    23/02/2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>

// My sockets library
#include "sockets.h"

#define SERVICE_PORT 8642
#define MAX_QUEUE 5
#define BUFFER_SIZE 1023

void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);

int main(int argc, char * argv[])
{
    int server_fd;
    
    printf("\n=== SERVER PROGRAM ===\n");

    if (argc != 2)
        usage(argv[0]);

    server_fd = startServer(argv[1]);
    
    printf("Server ready and waiting!\n");
    // Stand by to receive connections from the clients
    waitForConnections(server_fd);

    printf("Closing the server socket\n");
    // Close the socket
    close(server_fd);
    
    return 0;
}

// Show the user how to run this program
void usage(char * program)
{
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}



// Stand by for connections by the clients
void waitForConnections(int server_fd)
{
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connection_fd;
    pid_t pid;

    // Loop to wait for client connections
    while (1)
    {
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        connection_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if (connection_fd == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Identify the client
        // Get the ip address from the structure filled by accept
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received connection from '%s' on port '%d'\n", client_presentation, client_address.sin_port);
        
        pid = fork();
        // Parent process
        if (pid > 0)
        {
            close(connection_fd);
        }
        // Child process
        else if (pid == 0)
        {
            close(server_fd);
            // Establish the communication
            communicationLoop(connection_fd);
            exit(EXIT_SUCCESS);
        }
        // Fork error
        else
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

    }
}

// Do the actual receiving and sending of data
void communicationLoop(int connection_fd)
{
    char buffer[BUFFER_SIZE];
    //int message_counter = 0;
    int max_val;
    int number;
    int guess = 0;
    
    // Handshake
    receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    sprintf(buffer, "RANGE");
    sendMessage(connection_fd, buffer, strlen(buffer));
    
    // Receive the range from the user
    receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    sscanf(buffer, "%d", &max_val);
    // Start the game by generating the random number
    srand(time(NULL));
    number = rand() % max_val + 1;
    printf("Number generated is: %d\n", number);
    sprintf(buffer, "GUESS");
    sendMessage(connection_fd, buffer, strlen(buffer));

    while (guess != number)
    {
        if (!receiveMessage(connection_fd, buffer, BUFFER_SIZE))
        {
            // Exit the loop when the client disconnects
            break;
        }
        sscanf(buffer, "%d", &guess);
        printf("The client sent a %d\n", guess);
        
        if (guess == number)
            sprintf(buffer, "CORRECT");
        else if (guess < number)
            sprintf(buffer, "HIGHER");
        else
            sprintf(buffer, "LOWER");
        
        // Send a reply
        sendMessage(connection_fd, buffer, strlen(buffer));
    }
    
    // Goodbye
    receiveMessage(connection_fd, buffer, BUFFER_SIZE);
    sprintf(buffer, "BYE");
    sendMessage(connection_fd, buffer, strlen(buffer));
}
