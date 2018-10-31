/*
Program for an online BlackJack simulation (client side)

Emiliano Abascal Gurria A01023234
07/10/2018

This Program uses the sockets.c library created on class by Gilberto Echeverria.
*/
// include all the standard libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
// My libraries
#include "sockets.h"
#include "client.h"

#define SERVICE_PORT 8642
#define BUFFER_SIZE 1024


int main(int argc, char * argv[]){
    printf("\n=== Welcome ===\n");

    if (argc != 3)
        usage(argv[0]);

    connectToServer(argv[1], argv[2]);

    return 0;
}

// Show the user how to run this program
void usage(char * program){
    printf("Usage:\n%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

// Establish a connection with the server indicated by the parameters
void connectToServer(char * address, char * port){
    struct addrinfo hints;
    struct addrinfo * server_info;
    int connectionFD;

    // Prepare the information to determine the local address
    bzero(&hints, sizeof hints);
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    ///// GETADDRINFO
    // Get the actual address of this computer
    if (getaddrinfo(address, port, &hints, &server_info) == -1){
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    // SOCKET
    // Use the information obtained by getaddrinfo
    connectionFD = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (connectionFD == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // CONNECT
    // Connect with the desired port
    if (connect(connectionFD, server_info->ai_addr, server_info->ai_addrlen) == -1){
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Release the memory of the list of addrinfo
    freeaddrinfo(server_info);

    // Establish the communication
    communicationLoop(connectionFD);

    // Close the socket
    close(connectionFD);
}

// Do the actual receiving and sending of data
void communicationLoop(int connectionFD){
  //Declaration of all the needed variables.
    char buffer[BUFFER_SIZE];
    int chars_read;
    int input = 0;
    int victory = 0;
    char cards[BUFFER_SIZE];
    // Handshake
    sendMessage(connectionFD, buffer, strlen(buffer));
    receiveMessage(connectionFD, buffer, BUFFER_SIZE);
    printf("Welcome To BlackJack EAG, Are you ready to begin?(PRESS 1 TO CONTINUE): ");
    scanf("%d", &input);
    if(input == 1){
      sprintf(buffer, "%d", input);
      sendMessage(connectionFD, buffer, strlen(buffer));
    }
    //The initial cards are received and shown to the user.
    receiveMessage(connectionFD, buffer, BUFFER_SIZE);
    printf("This are your cards: %s\n", buffer);
    strcpy(cards, buffer);
    // The game starts by asking the user input.
    while (victory == 0){
        printf("\nWhat do you want to do? \n1.Hit\n2.Stand\n3.Exit(You Loose): ");
        scanf("%d", &input);
        sprintf(buffer, "%d", input);
        //The input is sent to the server and an answer is received.
        sendMessage(connectionFD, buffer, strlen(buffer));
        memset(buffer,0,strlen(buffer));
        chars_read = receiveMessage(connectionFD, buffer, BUFFER_SIZE);
        //if the response is LOST, then the client looses and it exits the loop, if the response is VICTORY, then the client wins, and exits the loop.
        if (strncmp(buffer, "LOST", 4) == 0){
            printf("This are your cards: %s\nYou Loose\n", cards);
            break;
        }
        else if(strncmp(buffer, "VICTORY", 7) == 0){
            printf("This are your cards: %s\nYou Won\n", cards);
            break;
        }
        printf("This are your cards: %s\n", buffer);
        strcpy(cards, buffer);
        // Finish the loop if the server disconnected.
        if (!chars_read){
            break;
          }
    }
    //Goodbye
    sprintf(buffer, "BYE");
    sendMessage(connectionFD, buffer, strlen(buffer));
    chars_read = receiveMessage(connectionFD, buffer, BUFFER_SIZE);

}
