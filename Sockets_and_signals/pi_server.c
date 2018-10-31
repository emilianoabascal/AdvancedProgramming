/*
    Emiliano Abascal Gurria A01023234

    Server program to compute the value of PI
    This program calls the library function 'get_pi'
    It will create child processes to attend requests
    It receives connections using sockets and will interrupt them using signals.

    Using Gilberto Echeverria's code from class
    14/10/2018
    14/10/2018
*/

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
#include "pi_server.h"

#include <ifaddrs.h>

#define BUFFER_SIZE 1024
#define MAX_QUEUE 5

///// MAIN FUNCTION
int main(int argc, char * argv[]){
    setupHandlers();
    int server_fd;

    printf("\nSERVER STARTING READY TO COMPUTE PI\n");
    // Check the correct arguments
    if (argc != 2){
        usage(argv[0]);
    }
    // Start the server
    server_fd = initServer(argv[1]);
	// Listen for connections from the clients
    waitForConnections(server_fd);
    // Close the socket
    close(server_fd);
    return 0;
}

///// FUNCTION DEFINITIONS

/*
    Explanation to the user of the parameters required to run the program
*/
void usage(char * program){
    printf("Usage:\n");
    printf("\t%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}

/*
    Prepare and open the listening socket
    Returns the file descriptor for the socket
    Remember to close the socket when finished
*/
int initServer(char * port){
    struct addrinfo hints;
    struct addrinfo * server_info = NULL;
    int server_fd;
    int reuse = 1;

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
    if (getaddrinfo(NULL, port, &hints, &server_info) == -1){
        fatalError("getaddrinfo");
    }

    // SOCKET
    // Open the socket using the information obtained
    server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (server_fd == -1){
        close(server_fd);
        fatalError("socket");
    }

    // SETSOCKOPT
    // Allow reuse of the same port even when the server does not close correctly
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof (int)) == -1){
        fatalError("setsockopt");
    }

    // BIND
    // Connect the port with the desired port
    if (bind(server_fd, server_info->ai_addr, server_info->ai_addrlen) == -1){
        fatalError("bind");
    }

    // LISTEN
    // Start listening for incomming connections
    if (listen(server_fd, MAX_QUEUE) == -1){
        fatalError("listen");
    }
    // FREEADDRINFO
    // Free the memory used for the address info
    freeaddrinfo(server_info);
    printf("Server listening\n");

    return server_fd;
}

/*
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd){
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pid_t new_pid;

    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1){
        // ACCEPT
        // Wait for a client connection
        client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
        if (client_fd == -1){
            fatalError("accept");
        }

        // Get the data from the client
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);

        // FORK
        // Create a new child process to deal with the client
        new_pid = fork();
        // Parent
        if (new_pid > 0){
            // Close the new socket
            close(client_fd);
        }else if (new_pid == 0){
            // Close the main server socket to avoid interfering with the parent
            close(server_fd);
            printf("Child process %d dealing with client\n", getpid());
            // Deal with the client
            attendRequest(client_fd);
            // Close the new socket
            close(client_fd);
            printf("Client Disconnecting\n");
            // Terminate the child process
            exit(EXIT_SUCCESS);
        }
        else{
            fatalError("fork");
        }
    }
}

/*
    Hear the request from the client and send an answer
*/
void attendRequest(int client_fd){
    //Declaration of a buffer to manage the messages sent and received with the client.
    char buffer[BUFFER_SIZE];
    int chars_read;
    //Variables that represent the current iteration and the iteration set by the client.
    unsigned long int currentIterations = 0;
    unsigned long int iterations;
    // Variables for polling
    struct pollfd test_fds;
    int poll_result;
    int timeout = 10;
    //values for the function of pi
    double pi_result = 4;
    int sign = -1;
    unsigned long int divisor = 3;

    // Clear the buffer
    bzero(buffer, BUFFER_SIZE);

    // Receive the request
    chars_read = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (chars_read == -1){
        fatalError("recv");
    }
    //Receive the number of iterations from the client and print them in the server.
    sscanf(buffer, "%lu", &iterations);
    printf("Client sent %lu iteration(s)\n", iterations);

    // Fill in the data for the structure
    // Polling stdin to check if the client wants to stop.
    test_fds.fd = client_fd;
    test_fds.events = POLLIN;
    poll_result = poll(&test_fds, 1, timeout);
    //While the current iterations are lesser than the iterations set by the user, then every thousand time the poll function will be called, this is because it would be very taxing to call it every time until the iterations are over. Then if the result of the poll is 0, then the operations of the original get_pi() function are done(i changed them so that all be done with only one loop instead of two, thats why the get_pi() funcitons are missing). Else if the result of the poll is greater than 0, the program will send the current value of PI and it's current iteration and exit the loop.
    while (currentIterations < iterations){
      if(currentIterations % 1000 == 0){
        poll_result = poll(&test_fds, 1, timeout);
      }
        if (poll_result == 0){
          pi_result += sign * (4.0/divisor);
          sign *= -1;
          divisor += 2;
          currentIterations++;
        }
        else if (poll_result > 0){
            if (test_fds.revents & POLLIN){
              sprintf(buffer, "%.20lf\n%lu", pi_result, currentIterations);
              if (send(client_fd, buffer, strlen(buffer) + 1, 0) == -1){
                  printf("Could not send reply");
              }
                break;
            }
        }else{
            // If poll detected an error due to an interruption, exit the loop
            if (errno == EINTR){
                break;
            }
        }
    }//End of loop
    //If the server is the one that stopped, then the result of pi is sent as well as the current iteration, else the calculation was finished and only the result is sent.
    if(currentIterations < iterations){
      sprintf(buffer, "%.20lf\n%lu", pi_result, currentIterations);
    }else{
      sprintf(buffer, "%.20lf", pi_result);
    }
    if (send(client_fd, buffer, strlen(buffer) + 1, 0) == -1){
        printf("Could not send reply");
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
