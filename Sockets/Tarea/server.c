 /*
    Program for an online BlackJack simulation (server side)

    Emiliano Abascal Gurria A01023234
    07/10/2018

    This Program uses the sockets.c library created on class by Gilberto Echeverria.
*/
// include all the standard libraries.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

// Include libraries for sockets
#include <netdb.h>
#include <arpa/inet.h>

// My libraries
#include "sockets.h"
#include "server.h"


#define SERVICE_PORT 8642
#define MAX_QUEUE 10
#define BUFFER_SIZE 1023


int main(int argc, char * argv[]){
    int serverFD;
    printf("\n=== BLACKJACK SERVER PROGRAM ===\n");

    if (argc != 2){
        usage(argv[0]);
      }

    serverFD = startServer(argv[1]);

    printf("WAITING FOR PLAYERS\n");
    // Stand by to receive connections from the clients
    waitForConnections(serverFD);

    printf("Closing the server socket\n");
    // Close the socket
    close(serverFD);
    return 0;
}

// Show the user how to run this program
void usage(char * program){
    printf("Usage:\n%s {port_number}\n", program);
    exit(EXIT_FAILURE);
}



// Stand by for connections by the clients
void waitForConnections(int server_fd){
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int connectionFD;
    pid_t pid;

    // Loop to wait for client connections
    while (1){
        ///// ACCEPT
        // Receive incomming connections
        // Get the size of the structure where the address of the client will be stored
        client_address_size = sizeof client_address;
        connectionFD = accept(server_fd, (struct sockaddr *) &client_address, &client_address_size);
        if (connectionFD == -1){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Identify the client
        // Get the ip address from the structure filled by accept
        inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
        printf("Received connection from '%s' on port '%d'\n", client_presentation, client_address.sin_port);

        pid = fork();
        // Parent process
        if (pid > 0){
            close(connectionFD);
        }
        // Child process
        else if (pid == 0){
            close(server_fd);
            // Establish the communication
            communicationLoop(connectionFD);
            exit(EXIT_SUCCESS);
        }
        // Fork error
        else{
            perror("fork");
            exit(EXIT_FAILURE);
        }

    }
}

// Do the actual receiving and sending of data
void communicationLoop(int connectionFD){
  srand(time(NULL));
  //Declaration of needed variables

    char buffer[BUFFER_SIZE];
    char temp[BUFFER_SIZE];
    int playerPoints = 0;
    int dealerPoints = 0;
    //i and j are iterators for the cards that have been played
    int i = 0;
    int j = 0;
    int x = 0;
    //input is the client input.
    int input = 0;
    card * playedCards;
    playedCards = (card*)malloc(sizeof(card));

    // Handshake
    receiveMessage(connectionFD, buffer, BUFFER_SIZE);
    sendMessage(connectionFD, buffer, strlen(buffer));


    receiveMessage(connectionFD, buffer, BUFFER_SIZE);
    // Start the game by dealing two cards to the dealer and two for the player, and sending the player cards to the client as well as only one of the cards of the dealer.
    //if a card is bigger than 10, then its value is 10, this cards are K, J and Q.
    //playedCards from 0 to 24 are the client's avalible cards, and from 26 to 51 are the dealer's avalible cards.
    if(strncmp(buffer, "1", 1) == 0){
      deal(playedCards, 0);
      if(playedCards[0].n > 10){
        playedCards[0].n = 10;
      }
      deal(playedCards, 1);
      if(playedCards[1].n > 10){
        playedCards[1].n = 10;
      }
      deal(playedCards, 25);
      if(playedCards[25].n > 10){
        playedCards[25].n = 10;
      }
      deal(playedCards, 26);
      if(playedCards[26].n > 10){
        playedCards[26].n = 10;
      }
      sprintf(buffer, "%d %d Dealer card %d", playedCards[0].n, playedCards[1].n, playedCards[25].n);
      playerPoints = playedCards[0].n + playedCards[1].n;
      dealerPoints = playedCards[25].n + playedCards[26].n;
      //Player and dealer points are printed in the server.
      printf("Player Points %d\n", playerPoints);
      printf("Dealer Points %d\n", dealerPoints);
      sendMessage(connectionFD, buffer, strlen(buffer));
      i = 2;
      j = 27;
    }
    //Loop to communicate with the client to actualy play the game, it receives answers from the client and sends outputs based on those  answers.
    while (x == 0){
      printf("The client sent a %d\n", input);

        if (!receiveMessage(connectionFD, buffer, BUFFER_SIZE)){
            // Exit the loop when the client disconnects
            break;
        }

        sscanf(buffer, "%d", &input);
        //if the player points are greater that 21 it looses, but if the dealer points are greater than 21, the client wins.
        if(playerPoints > 21){
          sprintf(buffer, "LOST");
          input = 0;
          //Message with the resulting information is sent.
          sendMessage(connectionFD, buffer, strlen(buffer));

        }else if(dealerPoints > 21){
          sprintf(buffer, "VICTORY");
          input = 0;
          //Message with the resulting information is sent.
          sendMessage(connectionFD, buffer, strlen(buffer));
        }

        //If the input was to take another card, then, if the dealer points are smaller than 17, then another card is dealt to the dealer.
        if(input == 1){
          if(dealerPoints < 17){
            deal(playedCards, j);
            dealerPoints = checkPoints(playedCards, 25, j);
            if(playedCards[j].n > 10){
              playedCards[j].n = 10;
            }
            j++;
          }
          //Card is dealed to the player and points based on the cards that the client has are calculated.
          deal(playedCards, i);
          if(playedCards[i].n > 10){
            playedCards[i].n = 10;
          }
          playerPoints = checkPoints(playedCards, 0, i);
          i++;

          //buffer is emptied to assure that junx is not being sent.
          memset(buffer,0,strlen(buffer));
          //The client's cards are assigned to the buffer.
          for(int j = 0; j < i; j++){
            sprintf(temp, "%d ", playedCards[j].n);
            strcat(buffer, temp);
          }
          //The dealer cards that can be shown are assigned to the buffer.
          strcat(buffer, "Dealer card ");
          for(int x = 25; x < j-1; x++){
            sprintf(temp, "%d ", playedCards[x].n);
            strcat(buffer, temp);
          }
          //The points of the dealer and the client are printed and the buffer sent to the client.
          printf("Player Points %d\n", playerPoints);
          printf("Dealer Points %d\n", dealerPoints);
          sendMessage(connectionFD, buffer, strlen(buffer));
        }
        //If the client decides to stand, then if the dealer points are smaller than 17, then another card is dealt to the dealer.
        if(input == 2){
          if(dealerPoints < 17){
            deal(playedCards, j);
            dealerPoints = checkPoints(playedCards, 25, j);
            j++;
          }
          //If the player Points are greater than 21 then the client automatically looses.
          if(playerPoints > 21){
            sprintf(buffer, "LOST");
            //Else if the player points are bellow or equal to 21 and the dealer points are greater than 21 the client wins.
          }else if(playerPoints <= 21 && dealerPoints > 21){
            sprintf(buffer, "VICTORY");
            //Else if the player points and the dealer points are bellow or equal 21 then it is calculated which is closer to 21, if the player is closed, then it wins, else it looses.
          }else if(playerPoints <= 21 && dealerPoints <= 21){
            playerPoints = 21 - playerPoints;
            dealerPoints = 21 - dealerPoints;
            if(playerPoints < dealerPoints){
              sprintf(buffer, "VICTORY");
            }else{
              sprintf(buffer, "LOST");
            }
          }
          //Message with the resulting information is sent.
          sendMessage(connectionFD, buffer, strlen(buffer));
        }
        //If the client decides to exit, then it looses.
        if(input == 3){
          sprintf(buffer, "LOST");
          sendMessage(connectionFD, buffer, strlen(buffer));

        }
    }
    // Goodbye
    receiveMessage(connectionFD, buffer, BUFFER_SIZE);
    sprintf(buffer, "BYE");
    sendMessage(connectionFD, buffer, strlen(buffer));
}
//Sum the points of a client or dealer. It receives an array of cards, the start index and the end index.
int checkPoints(card * cards, int start, int end){
  int result = 0;
  for(int j = start; j <= end; j++){
    result = result + cards[j].n;
  }
  return result;
}
//Deal cards, receives a position and a card array.
void deal(card * playedCards, int position){
  //Declaration of all the needed variables.
  int index = 0;
  char cardType[4] = {'D','S','C','H'};
  int cardNumber = 0;
  //Creates a random index to assign to the card's type.
  index = rand() % 3;
  //Creates a random number from 1 to 10, to assgn a card number to the card.
  cardNumber = 1 + rand() % 13;
  //Assigns the first card.
  if(position == 0){
    playedCards[position].n = cardNumber;
    playedCards[position].originalValue = cardNumber;
    playedCards[position].type = cardType[index];
    //if the first card has been dealt, then it creates a new card and checks if it has been dealt, if it was, then a new card is created until it has not been found, then it is assigned.
  }else{
    for(int j = 0; j <= 51; j++){
      if(playedCards[j].originalValue != cardNumber && playedCards[j].type != cardType[index]){
        playedCards[position].n = cardNumber;
        playedCards[position].originalValue = cardNumber;
        playedCards[position].type = cardType[index];
        break;
      }else{
        index = rand() % 3;
        cardNumber = 1 + (rand() % 13);
      }
    }
  }
}
