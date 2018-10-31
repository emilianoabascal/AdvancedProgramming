/*
   .h file to declare all the functions needed in server.c.

   Emiliano Abascal Gurria A01023234
   07/10/2018

   This Program uses the sockets.c library created on class by Gilberto Echeverria.
*/

#ifndef SERVER_H
#define SERVER_H
//card structure that contains a card value, an originalValue and a type, this is for the simulation of a standard deck.
typedef struct c{
  int n;
  int originalValue;
  char type;
}card;
void usage(char * program);
void waitForConnections(int server_fd);
void communicationLoop(int connection_fd);
int checkPoints(card * cards, int start, int end);
void deal(card * playedCards, int index);

#endif
