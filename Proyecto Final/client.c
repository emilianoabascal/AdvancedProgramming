/*
    Enrique Lira Martinez A01023351 ,Emiliano Abascal Gurria A01023234, Cesar Armando Valladares A01023503
    Client program to access nas
    This program connects to the server using sockets

    Gilberto Echeverria
    gilecheverria@yahoo.com
    29/03/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
// Sockets libraries
#include <netdb.h>
#include <sys/poll.h>
#include <arpa/inet.h>
// Custom libraries
#include "codes.h"
#include "client.h"
#include "sockets.h"
#include "fatal_error.h"
///// MAIN FUNCTION
int main(int argc, char * argv[])
{
    int connection_fd;

    printf("\n=== NAS CLIENT PROGRAM ===\n");

    // Check the correct arguments
    if (argc != 3)
    {
        usage(argv[0]);
    }

    // Start the server
    connection_fd = connectSocket(argv[1], argv[2]);
	// Use the bank operations available
    bankOperations(connection_fd);
    // Close the socket
    close(connection_fd);

    return 0;
}

void usage(char * program)
{
    printf("Usage:\n");
    printf("\t%s {server_address} {port_number}\n", program);
    exit(EXIT_FAILURE);
}

void bankOperations(int connection_fd){
    //Variable declaration
    long bytesTransfered = 0;
    int fileMaxSize = 1024;
    int second_option=0;
    int status = 10;
    char other_account;
    char option = 'p';
    char * fileName, * buffer = NULL;
    float balance;
    long numbytes =0;
    userStruct user;
    operation_t operation;
    unsigned char *packet = NULL;
    char * dirs;
    FILE *infile;
    struct pollfd stdin_fds[1];
    struct pollfd server_fds[1];
    int timeout = 10; // 10ms tiemout
    int poll_stdin;
    int poll_server;
    
    //Allocate memory for all the necessary 
    buffer = (char*)calloc(BUFFER_SIZE, sizeof(char));
    packet = (unsigned char*)calloc(fileMaxSize, sizeof(unsigned char));
    fileName = (char*)calloc(BUFFER_SIZE, sizeof(char));//Memory allocation for the file name.
    dirs = (char*)calloc(BUFFER_SIZE, sizeof(char));
    user.password = (char *)calloc(20, sizeof(char *));//create a variable for the password
    user.homeDirectory = (char *)calloc(20, sizeof(char *));//create a variable for the password
    user.optionalFilePath = (char *)calloc(BUFFER_SIZE, sizeof(char *));//create a variable for the password
    user.optionalFileName = (char *)calloc(BUFFER_SIZE, sizeof(char *));//create a variable for the password
    fileName = (char *)calloc(BUFFER_SIZE, sizeof(char *));
    while (option != 'w')
    {
      printf("Login:\n");
      printf("Select an option: \n");
      printf("\tc. Access account\n");
      printf("\tr. Register account\n");
      printf("\tx. Exit program\n");
    while(1){
            //this is a double poll, with this the client can be notice when the server had shut down.
            stdin_fds[0].fd = stdin->_file;// Poll for the input from the user
            stdin_fds[0].events = POLLIN;
            server_fds[0].fd = connection_fd;//this poll is for the server to listen a signal from it
            server_fds[0].events = POLLIN;

            poll_stdin = poll(stdin_fds, 1, timeout);
            if(poll_stdin == -1){
                fatalError("POLL");
            }
            else if(poll_stdin == 0){
                poll_server = poll(server_fds, 1, timeout);
                if(poll_server == -1){
                    fatalError("SERVER POLL");
                }else if(poll_server == 0){
                    // Nothing
                    continue;
                }else{
                    // Receive the response
                    if ( !recvString(connection_fd, buffer, BUFFER_SIZE) ){
                        printf("\nServer finich conection\n");
                        exit(0);
                    }
                    // Get the data
                    sscanf(buffer, "%d %f", &status, &balance);

                    if(status == BYE){
                        printf("\nSHUTTING DOWN!\n");
                        exit(0);
                    }
                }
                continue;
            }else{
              scanf("%c", &option);//get the client option
              break;
            }
        }
        
        switch(option){
            case 'c'://Login option
                printf("Enter account: ");//ask for the account user
                scanf("%d", &user.account);//store the account user
                printf("Enter password: ");//ask for the account password
                scanf("%s", user.password);//store the account password
                operation = VERIFY;//store the option to verify the account and password
                break;
            // Deposit into account
            case 'r'://Register option
                printf("Enter password ");
                scanf("%s", user.password);
                operation = REGISTER;
                break;
            // Deposit into account
            case 'x'://Exit option
                printf("Thanks for using the program. Bye!\n");
                break;
            default:
                printf("Invalid option. Try again ...\n");// Skip the rest of the code in the while
                continue;
        }
        if(option == 'x')//exit the while
        break;
        sprintf(buffer, "%d %s %d", operation, user.password, user.account);//store in the buffer the password and account
        sendString(connection_fd, buffer);//send buffer to server
        // Receive the response
        if ( !recvString(connection_fd, buffer, BUFFER_SIZE) )//recive message from server
        {
            printf("Server closed the connection\n");
            break;
        }
        // get the data
        sscanf(buffer, "%d %d", &status, &user.account);//store the values from buffer in status and account

          switch (status)
          {
            case CORRECT://you cant access to the system
                printf("Welcome to the NAS system\n");
                option = 'w';
                break;
            case REGISTERS://the registration was made correct
                printf("Your registration was made successfully\n");
                printf("you are the user number : %d\n", user.account);
                break;
            case INCORRECT://Incorrect account or password
                printf("\tIcorrect password or account\n");
                break;
        }

    }
    operation = 0;
    status = 0;
    sprintf(user.homeDirectory, "ServerFiles/%d/", user.account);// store the path for the user
    while (option != 'x'){
        printf("\nUSER: %d\n", user.account);
        printf("Menu:\n");
        printf("Select an option: \n");
        printf("\tu. Upload File\n");
        printf("\tp. Delete file\n");
        printf("\td. Download File\n");
        printf("\tl. Get Directories\n");
//        printf("\tq. Check permissions\n");
        printf("\tg. Give permissions\n");
        printf("\tr. Revoke permissions\n");
        printf("\tx. Exit program\n");

    while(1){
            //this is a double poll, with this the client can be notice when the server had shut down.
            stdin_fds[0].fd = stdin->_file;// Poll for the input from the user
            stdin_fds[0].events = POLLIN;
            server_fds[0].fd = connection_fd;//this poll is for the server to listen a signal from it
            server_fds[0].events = POLLIN;

            poll_stdin = poll(stdin_fds, 1, timeout);
            if(poll_stdin == -1){
                fatalError("POLL");
            }
            else if(poll_stdin == 0){
                poll_server = poll(server_fds, 1, timeout);
                if(poll_server == -1){
                    fatalError("SERVER POLL");
                }else if(poll_server == 0){
                    // Nothing
                    continue;
                }else{
                    // Receive the response
                    if ( !recvString(connection_fd, buffer, BUFFER_SIZE) ){
                        printf("\nServer finich conection\n");
                        exit(0);
                    }
                    // Get the data
                    sscanf(buffer, "%d %f", &status, &balance);

                    if(status == BYE){
                        printf("\nSHUTTING DOWN!\n");
                        exit(0);
                    }
                }
                continue;
            }else{
                scanf(" %c", &option);//get the client opction
                break;
            }
        }

        // Init variables to default values
        balance = 0;
        bytesTransfered = 0;
        switch(option){//Switch to add menu functionality.
            case 'u'://Case to upload files
                printf("Enter File Name: ");//Scan for the file name.
                scanf("%s", fileName);
                while (access(fileName, F_OK) == -1){//While the file does not exist, keep asking.
                  printf("File Not Found\nTry again: ");//Scan for the file name.
                  scanf("%s", fileName);
                }
                infile = fopen(fileName, "rb");//Read File
                //Get the numner of bytes
                fseek(infile, 0L, SEEK_END);
                numbytes = ftell(infile);
                fseek(infile, 0L, SEEK_SET);
                fclose(infile);//Close the file.
                operation = RECEIVEFILE;//Set the operation that the client wants the server to do.
                sprintf(buffer, "%d %f %ld %s %s", operation, balance,numbytes, fileName, user.homeDirectory);//Parse the variables into the buffer.
                break;
            case 'l'://Case to show the files in the home directory of the user.
                operation = GETDIRECTORIES;//Set the operation that the client wants the server to do.
                sprintf(buffer, "%d %f %ld %s %s", operation, 0.0, numbytes, "|", user.homeDirectory);//Parse the variables into the buffer.
                break;
            case 'd':
              operation = SENDFILE;//Set the operation that the client wants the server to do.
              printf("Enter File Name: ");
              scanf("%s", fileName);
              sprintf(buffer, "%d %f %ld %s %s", operation, balance,numbytes, fileName, user.homeDirectory);//Parse the variables into the buffer.
              break;
            case 'q':
                operation = CHECKA;//Set the operation that the client wants the server to do.
                sprintf(buffer, "%d", operation);
                break;
            case 'p':
                operation = DELETE;//Set the operation that the client wants the server to do.
                sprintf(buffer, "%d", operation);
                break;
            case 'r':
                operation = REVOKE;//Set the operation that the client wants the server to do.
                sprintf(buffer, "%d", operation);
                break;
            case 'g':
                operation = GIVE;//Set the operation that the client wants the server to do.
                sprintf(buffer, "%d", operation);
                break;
            case 'x':
                printf("Thanks for using the program. Bye!\n");
                operation = EXIT;//Set the operation that the client wants the server to do.
                sprintf(buffer, "%d", operation);
                break;
            default:// Incorrect option
                printf("Invalid option. Try again ...\n");
                continue;
        }
        
        if(operation != BACKTOMENU){
          sendString(connection_fd, buffer);//Send the buffer to the server.
          // Receive the response
          if ( !recvString(connection_fd, buffer, BUFFER_SIZE) )//Receive the server's response to know what to to next.
          {
            printf("Server closed the connection\n");
            break;
          }
          // get the data
        sscanf(buffer, "%d %ld", &status, &numbytes);
        switch (status){//Switch to know what operation to do in the client's side.
//            case ASK_ACCOUNT://give the account you want to check o add to yotu list of access(Pending)
//                printf("Enter account: ");
//                scanf("%s", &other_account);//ask the account
//                sprintf(buffer, "%s", &other_account);//store in the buffer
//                sendString(connection_fd, buffer);//send buffer
//                recvString(connection_fd, buffer, BUFFER_SIZE);
//                sscanf(buffer, "%i",&second_option);//recive the answer
//                if(second_option == WRITTEN){//this is only for the write of the account in the access file
//                  printf("\tIt was written successfully\n");
//                  break;
//                }else{
//                  if (second_option == FOUND){//this for the part to check if you hver permissions
//                  printf("You have access\n" );
//                  }else{
//                  printf("You do not have access\n" );
//                  break;
//                  }
//                }
//                sscanf(buffer, "");
//                sscanf(fileName, "");
            case CLIENTUPLOADFILE://Case to upload file to the server.
                if(uploadFile(numbytes, 0, packet, connection_fd, fileName)){//If the file was upladed then it is reported to the client in a message of success.
                  printf("File was Uploaded correctly\n");
                }else{
                  printf("There was a problem\n");//If the file was not uploaded then it is reported to the client in a message of error.
                }
                //Variables used in other functions are set to 0;
                numbytes = 0;
                sscanf(fileName, "");
                break;
            case CLIENTRECIVEFILE://Case to Download files from the server.
              if(downloadFile(numbytes, 0, 0, packet, connection_fd, fileName)){//If the file was downloaded then it is reported to the client in a message of success.
                printf("\nFile was Downloaded correctly\n");
              }else{
                printf("\nThere was a problem\n");//If the file was not downloaded then it is reported to the client in a message of error.
              }
              //Variables used in other functions are set to 0;
              sscanf(fileName, "");
              numbytes = 0;
              break;
            case CLIENT_SHOW_DIRECTORIES://See the files and directory of a directory.
                recvString(connection_fd, buffer, BUFFER_SIZE);//Receive the buffer.
                sscanf(buffer, "%s", dirs);
                printf("\t%s\n", buffer);//Print the directories sent from the server.
                sscanf(buffer, "");
                break;
            case NAMEFILE://ask only for the name of the file
                printf("Enter File Name: ");
                scanf("%s",fileName);
                sprintf(buffer, "%s %s", fileName,"ServerFiles/");//path from user
                sendString(connection_fd, buffer);//send buffer
                if (!recvString(connection_fd, buffer, BUFFER_SIZE)){
                    printf("Server closed the connection\n");
                    break;
                }
                sscanf(buffer, "%i",&second_option);//recive answer
                if(second_option == DELOK){
                  printf("file deleted successfully.\n");//if the file was found it will be delete
                }else{//if the file wasn't found it won't be delete
                  printf("Unable to delete the file\n");
                  perror("Following error occurred");
                }
                sscanf(fileName, "");
                sscanf(buffer, "");
                break;
            case BYE:
                //Free memory
                free(packet);
                free(fileName);
                free(dirs);
                free(buffer);
                free(user.password);
                free(user.optionalFileName);
                free(user.optionalFilePath);
                free(user.homeDirectory);
                printf("\tThanks for connecting to the server. Good bye!\n");//end connection
                break;
            case ERROR: default:
                printf("\tInvalid operation. Try again\n");//error
                break;
           
        }
    }
}

bool downloadFile(float numbytes, float bytesTransfered, float bytesReceived, unsigned char * packet, int connection_fd, char * fileName){
  FILE *fp = fopen(fileName, "wb+");//Open file in which the arriving value will be saved and While the size of the Arriving file is greater than bytes transfered, then write the arriving bytes into the file.
  while(numbytes > bytesTransfered){
    bytesReceived = read(connection_fd, packet, 1024);
    printf("\rReceived: %f MB of %f MB.",bytesTransfered/1000000, numbytes/1000000);
  	fflush(stdout);
    fwrite(packet, 1,bytesReceived,fp);
    bytesTransfered += bytesReceived;
  }
  printf("\rReceived: %f Mb.",(bytesTransfered/1000000));
  fflush(stdout);
  if(fwrite(packet, 1,bytesReceived,fp)){//If the last part of the file was written return success.
    fclose(fp);
    return true;
  }else{//else return false.
    return false;
  }
}

bool uploadFile(float numbytes, float bytesTransfered, unsigned char * packet, int connection_fd, char * filename){
  FILE *infile = fopen(filename, "rb+");//Open file that is going to be uploaded.
  while (bytesTransfered < numbytes){//While the size of the file being sent is greater than the bytes sent in every iteration, then send a determinate number of bytes.
    int nread = fread(packet,1,1024,infile);
    bytesTransfered += nread;
    write(connection_fd, packet, nread);
    printf("\rSending... %f MB of %f MB.", bytesTransfered/1000000, numbytes/1000000);
    fflush(stdout);
  }
  fclose(infile);//Close bytes and send success if the file arrived to the server.
  if(bytesTransfered >= numbytes){
    return true;
  }else{
    return false;
  }
}
