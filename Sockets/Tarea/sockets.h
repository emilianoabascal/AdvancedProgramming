/*
.h file to declare all the functions needed in server.c.

Emiliano Abascal Gurria A01023234
07/10/2018

This Program uses the sockets.c library created on class by Gilberto Echeverria.
*/

#ifndef SOCKETS_H
#define SOCKETS_H

// Receive a message and validate the return values
int receiveMessage(int connection_fd, char * buffer, int buff_length);


// Send a message and validate
void sendMessage(int connection_fd, char * buffer, int buff_length);

int startServer(char * port);


#endif
