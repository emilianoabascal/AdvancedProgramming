/*
   .h file to declare all the functions needed in client.c.

   Emiliano Abascal Gurria A01023234
   07/10/2018

   This Program uses the sockets.c library created on class by Gilberto Echeverria.
*/

#ifndef CLIENT_H
#define CLIENT_H

void usage(char * program);
void connectToServer(char * address, char * port);
void communicationLoop(int connection_fd);


#endif
