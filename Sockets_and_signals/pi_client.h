#ifndef PI_CLIENT_H
#define PI_CLIENT_H

#include <stdio.h>
#include <stdlib.h>


// FUNCTION DECLARATIONS
int interrupted = 0;
void usage(char * program);
int openSocket(char * address, char * port);
void requestToStop(int server_fd);
void fetchResult(int connection_fd);
void setupHandlers();
void detectInterruption(int signal);


#endif  /* NOT PI_CLIENT_H */
