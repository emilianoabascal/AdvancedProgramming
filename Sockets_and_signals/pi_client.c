/*
    Emiliano Abascal Gurria A01023234

    Client program to compute the value of PI
    This program calls the library function 'get_pi'
    It sends connections using sockets for the server program to handle them.

    Using Gilberto Echeverria's code from class
    14/10/2018
*/
//Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Sockets libraries
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <signal.h>
#include <errno.h>
// Custom libraries
#include "fatal_error.h"
#include "pi_client.h"
#include <ifaddrs.h>
//Constant definition for the buffer size.
#define BUFFER_SIZE 1024
///// MAIN FUNCTION
int main(int argc, char * argv[]){
    setupHandlers();
    int connection_fd;
    printf("\n=== WELCOME, lets crunch pi :D ===\n");
    // Check the correct arguments
    if (argc != 3){
        usage(argv[0]);
    }
    // Start the server
    connection_fd = openSocket(argv[1], argv[2]);
	// Listen for connections from the clients
    fetchResult(connection_fd);
    // Close the socket
    close(connection_fd);

    return 0;
}

///// FUNCTION DEFINITIONS

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program){
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*
    Open the socket to the server
    Returns the file descriptor for the socket
    Remember to close the socket when finished
*/
int openSocket(char * address, char * port){
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;
    int connection_fd;

    // Prepare the hints structure
    // Clear the structure for the server configuration
    bzero(&hints, sizeof hints);
    // Set to use IPV4
    hints.ai_family = AF_INET;
    // Use stream sockets
    hints.ai_socktype = SOCK_STREAM;
    // Get the local IP address automatically
    hints.ai_flags = AI_PASSIVE;

    // GETADDRINFO
    // Use the presets to get the actual information for the socket
    // The result is stored in 'server_info'
    if (getaddrinfo(address, port, &hints, &server_info) == -1){
        fatalError("getaddrinfo");
    }

    // SOCKET
    // Open the socket using the information obtained
    connection_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (connection_fd == -1){
        close(connection_fd);
        fatalError("socket");
    }

    // CONNECT
    // Connect to the server
    if (connect(connection_fd, server_info->ai_addr, server_info->ai_addrlen) == -1){
        fatalError("connect");
    }

    // FREEADDRINFO
    // Free the memory used for the address info
    freeaddrinfo(server_info);

    return connection_fd;
}
//Function to send the iterations to the server and receive a result.
void fetchResult(int connection_fd){
    //Declaration of a buffer to manage the messages sent and received with the client.
    char buffer[BUFFER_SIZE];
    int chars_read;
    //Variables that represent iteration set by the client, a result on which the value will be stored and a iteration that will represent when the program stopped.
    double result;
    unsigned long int iterations;
    int iterationStop = 0;
    // Variables for polling
    struct pollfd test_fds;
    int poll_result;
    int timeout = 10;
    //The iterations are asked to the user.
    printf("Enter the number of iterations you want for pi: ");
    scanf("%lu", &iterations);
    sprintf(buffer, "%lu", iterations);
    // Send the iterations to the server.
    if (send(connection_fd, buffer, strlen(buffer)+1, 0) == -1){
        fatalError("send");
    }
    // Fill in the data for the structure
    test_fds.fd = connection_fd;// Polling stdin
    test_fds.events = POLLIN;
    // Compute the value of PI
    while (!interrupted){
        poll_result = poll(&test_fds, 1, timeout);
        //If the poll result is greater than 0, then the server sent a response, else The poll returned an error.
        if (poll_result == 0){
        }else if (poll_result > 0){
            if (test_fds.revents & POLLIN){
                break;
            }
        }else{
            // If poll detected an error due to an interruption, exit the loop
            if (errno == EINTR){
              //if there is a exit singal, it  request the current value and break
              requestToStop(connection_fd);
              break;
            }
        }
    }

    // Clear the buffer
    bzero(buffer, BUFFER_SIZE);
    // Receive the result of crunching pi.
    chars_read = recv(connection_fd, buffer, BUFFER_SIZE, 0);
    if (chars_read == -1){
        fatalError("recv");
    }
    sscanf(buffer, "%lf\n%u", &result, &iterationStop);
    //if the calculation was complete, then only the result is printed, else the result and the iterations are.
    if(iterationStop == 0){
      printf("The value for PI is: %.20lf\nDisconnecting\n", result);

    }else{
      printf("The value for PI is: %.20lf and it was stopped in the iteration %d.\nDisconnecting\n", result, iterationStop);
    }

}
//Function to exit the server.
void requestToStop(int server_fd){
    if (send(server_fd, "q", 1 + 1, 0) == -1){
        fatalError("send");
    }
}
// Signal handler
void setupHandlers(){
    struct sigaction new_action;
    // Configure the action to take
    // Block all signals during the time the handler funciton is running
    sigfillset(&new_action.sa_mask);
    new_action.sa_handler = detectInterruption;

    // Set the handler
    sigaction(SIGINT, &new_action, NULL);
}


void detectInterruption(int signal){
    interrupted = 1;
}
