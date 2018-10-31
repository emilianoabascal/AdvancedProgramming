/*

    Emiliano Abascal Gurria A01023234
    Program for a simple bank server
    It uses sockets and threads
    The server will process simple transactions on a limited number of accounts

    Gilberto Echeverria
    gilecheverria@yahoo.com
    21/10/2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Signals library
#include <errno.h>
#include <signal.h>
// Sockets libraries
#include <netdb.h>
#include <sys/poll.h>
// Posix threads library
#include <pthread.h>

// Custom libraries
#include "bank_codes.h"
#include "sockets.h"
#include "fatal_error.h"
#include "bank_server.h"
///// MAIN FUNCTION
int main(int argc, char * argv[]){
    int server_fd;
    bankType bankData;
    locksType dataLocks;

    printf("\n=== SIMPLE BANK SERVER ===\n");

    // Check the correct arguments
    if (argc != 2){
        usage(argv[0]);
    }

    // Configure the handler to catch SIGINT
    setupHandlers();

    // Initialize the data structures
    initBank(&bankData, &dataLocks);

	// Show the IPs assigned to this computer
	printLocalIPs();
    // Start the server
    server_fd = initServer(argv[1], MAX_QUEUE);
	// Listen for connections from the clients
    waitForConnections(server_fd, &bankData, &dataLocks);
    // Close the socket
    close(server_fd);

    // Clean the memory used
    freeBankMemory(&bankData, &dataLocks);

    // Finish the main thread
    pthread_exit(NULL);

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
    Modify the signal handlers for specific events
*/
void setupHandlers() {
    signal(SIGINT, exitHandler);
}


/**
 * SIGINT handler
 */
 void exitHandler(int signal){
     // Set flag to true
     exitFlag = 1;
 }
/*
    Function to initialize all the information necessary
    This will allocate memory for the accounts, and for the mutexes
*/
void initBank(bankType * bankData, locksType * dataLocks){
    // Set the number of transactions
    bankData->totalTransactions = 0;

    // Allocate the arrays in the structures
    bankData->accountArray = malloc(NUM_ACCOUNTS * sizeof (accountType));
    // Allocate the arrays for the mutexes
    dataLocks->accountMutex = malloc(NUM_ACCOUNTS * sizeof (pthread_mutex_t));

    // Initialize the mutexes, using a different method for dynamically created ones
    //dataLocks->transactionsMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_init(&dataLocks->transactionsMutex, NULL);
    for (int i=0; i<NUM_ACCOUNTS; i++)
    {
        //dataLocks->accountMutex[i] = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_init(&dataLocks->accountMutex[i], NULL);
    }
}

/*
    Main loop to wait for incomming connections
*/
void waitForConnections(int server_fd, bankType * bankData, locksType * dataLocks){
    struct sockaddr_in client_address;
    socklen_t client_address_size;
    char client_presentation[INET_ADDRSTRLEN];
    int client_fd;
    pthread_t new_tid;
    int poll_response;
	  int timeout = 500;		// Time in milliseconds (0.5 seconds)
    // Get the size of the structure to store client information
    client_address_size = sizeof client_address;

    while (1){
		//// POLL
        // Create a structure array to hold the file descriptors to poll
        struct pollfd test_fds[1];
        // Fill in the structure
        test_fds[0].fd = server_fd;
        test_fds[0].events = POLLIN;    // Check for incomming data
        // Check if there is any incomming communication
        poll_response = poll(test_fds, 1, timeout);
		    // Error when polling
        if (poll_response == -1){
            // SIGINT will trigger this
            // errno is used to check why is the program exiting, if it is an error then fatalError is called, else the program is exitting due to a signal.
            if(errno == EINTR && exitFlag){
                printf("\n\nThe server is about to shut down - No longer listening.\n");
                break;
            }else{
                fatalError("poll");
            }
        }else if (poll_response == 0){// Timeout finished without reading anything
            // The exit flag has been activated => stop listening for requests
            if(exitFlag){
                printf("\n\nThe server is about to shut down - No longer listening.\n");
                break;
            }
        }
		    // There is something ready at the socket
        else{
            // Check the type of event detected
            if (test_fds[0].revents & POLLIN){
				// ACCEPT
				// Wait for a client connection
				client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_size);
				if (client_fd == -1){
					fatalError("Error: accept");
				}

				// Get the data from the client
				inet_ntop(client_address.sin_family, &client_address.sin_addr, client_presentation, sizeof client_presentation);
				printf("Received incomming connection from %s on port %d\n", client_presentation, client_address.sin_port);
				// Prepare the structure to send to the thread
				// CREATE A THREAD
                connectionDataType* temp_data = malloc(sizeof(connectionDataType));
                temp_data->bankData = bankData;
                temp_data->dataLocks = dataLocks;
                temp_data->fd = client_fd;
                int status;
                status = pthread_create(&new_tid, NULL, attentionThread, (void*) temp_data);
                if(status != 0){
                    printf("Failed to create handler!\n");
                }
                else{
                    printf("Created thread %lu for request.\n", new_tid);
                }
            }
        }
    }
    // Print the total number of transactions performed
    printf("The server made: %i transactions.\n", numberOfTransacrions(bankData, &(dataLocks->transactionsMutex)));

}

/*
    Hear the request from the client and send an answer
*/
void * attentionThread(void * arg) {
    connectionDataType* data = (connectionDataType*) arg;
    printf("Attending request %lu\n", pthread_self());
    // Stuff required by poll
    struct pollfd test_fds[1];
    int timeout = 500; // 500ms tiemout
    int poll_result;
    char buffer[BUFFER_SIZE];

    //Account stuff
    float balance = 0;
    withdrawStatusType withraw_status;

    while (!exitFlag){
        // POLL
        // Fill in the data for the structure
        test_fds[0].fd = data->fd;
        test_fds[0].events = POLLIN;
        // Call poll
        poll_result = poll(test_fds, 1, timeout);

        if (poll_result == -1){
            // Check SIGINT
            // If the errno is a signal then exit because of the signal, else break because of an error.
            if(errno == EINTR && exitFlag){
                printf("Poll killed by exit flag!\n");
                // An exit signal got us here, therefore we must break
                break;
            }else{
                fatalError("poll");
            }
        }else if (poll_result == 0){
            continue;
        }else{
            // Receive message
            if(recvString(data->fd, buffer, BUFFER_SIZE) == 0){
                printf("Client disconnected!\n");
                break;
            }
            incommingMessageType msg;
            parseIncomming(&msg, buffer);
            // Client exiting procedure
            if(msg.op == EXIT){
                printf("Client is leaving!\n");
                break;
            }

            // If the account is not valid, then a response is sent indicating that is not valid.
            if(!accountValidation(msg.account)){
                sendResponse(data->fd, NO_ACCOUNT, 0);
                continue;
            }
            switch(msg.op){
                case CHECK://Get account balance
                    balance = getBalance(data, msg.account);
                    break;
                case DEPOSIT:
                    //If the value is not valid, then senf error, then make deposit.
                    if(msg.value < 0){
                        sendResponse(data->fd, ERROR, 0);
                        continue;
                    }
                    balance = deposit(data, msg.account, msg.value);
                    break;
                case WITHDRAW://
                    // If the value is not valid, then senf error, then call the function withdraw
                    if(msg.value < 0){
                        sendResponse(data->fd, ERROR, 0);
                        continue;
                    }
                    withdraw(&withraw_status, data, msg.account, msg.value);
                    //Withdraw Error.
                    if(withraw_status.error){
                        sendResponse(data->fd, INSUFFICIENT, 0);
                        continue;
                    }else{// Widthraw success
                        balance = withraw_status.balance;
                    }
                    break;
                default:
                    fatalError("Unknown code!");
            }
            // The balance is sent if there was no errors.
            sendResponse(data->fd, OK, balance);
            printf("Transaction succesfull - Total: %i\n", threadTransactions(data));
        }
    }
    //Tell the client that the server is closing.
    sendResponse(data->fd, BYE, 0);
    // Free the struct used to pass massages onto the thread
    free(data);
    printf("Closing thread %lu\n", pthread_self());
    pthread_exit(NULL);
}

/*
    Free all the memory used for the bank data
*/
void freeBankMemory(bankType * bankData, locksType * dataLocks) {
    free(bankData->accountArray);
    free(dataLocks->accountMutex);
    printf("Thread Memory was released\n");

}

/*
    Return true if the account provided is within the valid range,
    return false otherwise
*/

//Function to send a response to the client.
void sendResponse(int connectionFd, responseTransaction code, float balance){
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    sprintf(buffer, "%i %f", (int)code, balance);
    sendString(connectionFd, buffer);
}


int accountValidation(int account){
    return (account >= 0 && account < NUM_ACCOUNTS);
}

//Function to get an account's balance.
float getBalance(connectionDataType* data, int accountNumber){
    accountType* account = &(data->bankData->accountArray[accountNumber]);
    pthread_mutex_t* accountLock = &(data->dataLocks->accountMutex[accountNumber]);
    pthread_mutex_t* transactionLock = &(data->dataLocks->transactionsMutex);
    float value;
    // Locks the account and transactions to avoid sending wrong information if a transaction is made with the account at the same time.
    pthread_mutex_lock(accountLock);
    pthread_mutex_lock(transactionLock);
    // Checks the Balance and ulocks the account and the transaction.
    value = account->balance;
    data->bankData->totalTransactions++;
    //The transaction and the account are unlocked.
    pthread_mutex_unlock(transactionLock);
    pthread_mutex_unlock(accountLock);
    //Returns the balance.
    return value;
}

//Function to deposit to an account.
float deposit(connectionDataType* data, int accountNumber, float amount){
    accountType* account = &(data->bankData->accountArray[accountNumber]);
    pthread_mutex_t* accountLock = &(data->dataLocks->accountMutex[accountNumber]);
    pthread_mutex_t* transactionLock = &(data->dataLocks->transactionsMutex);
    float value;
    // Locks the account and transactions to avoid sending wrong information if a transaction is made with the account at the same time.
    pthread_mutex_lock(accountLock);
    pthread_mutex_lock(transactionLock);
    // Adds the account's balance and the deposit amount updating the balance, and ulocks the account and the transaction.
    account->balance += amount;
    value = account->balance;
    data->bankData->totalTransactions++;
    //The transaction and the account are unlocked.
    pthread_mutex_unlock(transactionLock);
    pthread_mutex_unlock(accountLock);
    //Returns the updated balance.
    return value;
}

//Function to withdraw money from an account.

void withdraw(withdrawStatusType* status, connectionDataType* data, int accountNumber, float amount){
    accountType* account = &(data->bankData->accountArray[accountNumber]);
    pthread_mutex_t* accountLock = &(data->dataLocks->accountMutex[accountNumber]);
    pthread_mutex_t* transactionLock = &(data->dataLocks->transactionsMutex);

    // Locks the account and transactions to avoid sending wrong information if a transaction is made with the account at the same time.
    pthread_mutex_lock(accountLock);
    pthread_mutex_lock(transactionLock);
    //If account balance is enough, then the withdraw can take place, the amount that will be withrawned is substracted from the accounts balance, if is nit enough, then the balance is unchanged and an error is saved so that the server can notify the client.
    if(account->balance >= amount){
        account->balance -= amount;
        data->bankData->totalTransactions++;
        status->balance = account->balance;
        status->error = 0;
    }else{
        //status->balance = 0;
        status->error = 1;
    }
    //The transaction and the account are unlocked.
    pthread_mutex_unlock(transactionLock);
    pthread_mutex_unlock(accountLock);
}

//Function to get the transactions made by each thread.
int threadTransactions(connectionDataType* data){
    return numberOfTransacrions(data->bankData, &(data->dataLocks->transactionsMutex));
}

//Funtion to get the number of transactions made by the server.
int numberOfTransacrions(bankType* bankData, pthread_mutex_t* transactionLock){
    int value;
    pthread_mutex_lock(transactionLock);
    value = bankData->totalTransactions;
    pthread_mutex_unlock(transactionLock);
    return value;
}

//Function to format the client's message.
void parseIncomming(incommingMessageType* msg, char* s){
    sscanf(s, "%i %i %f", (int*)&(msg->op), &(msg->account), &(msg->value));
}
