#ifndef PI_SERVER_H
#define PI_SERVER_H

#include <stdio.h>
#include <stdlib.h>

// FUNCTION DECLARATIONS
int interrupted = 0;
void usage(char * program);
void printLocalIPs();
int initServer(char * port);
void waitForConnections(int server_fd);
void attendRequest(int client_fd);
void setupHandlers();
void detectInterruption(int signal);

#endif  /* NOT PI_SERVER_H */
