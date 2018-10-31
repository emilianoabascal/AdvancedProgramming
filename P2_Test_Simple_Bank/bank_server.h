/*
    Emiliano Abascal Gurria A01023234
    21/10/2018
*/

#ifndef BANK_SERVER_H
#define BANK_SERVER_H
#define NUM_ACCOUNTS 4
#define BUFFER_SIZE 1024
#define MAX_QUEUE 5

#include <pthread.h>
#include "bank_codes.h"
#include "sockets.h"
int exitFlag = 0;
// Data for a single bank account
typedef struct accountStruct {
    int id;
    float balance;
} accountType;

// Data for the bank operations
typedef struct bankStruct {
    // Store the total number of operations performed
    int totalTransactions;
    // An array of the accounts
    accountType * accountArray;
} bankType;

// Structure for the mutexes to keep the data consistent
typedef struct locksStruct {
    // Mutex for the number of transactions variable
    pthread_mutex_t transactionsMutex;
    // Mutex array for the operations on the accounts
    pthread_mutex_t * accountMutex;
} locksType;

// Data that will be sent to each structure
typedef struct dataStruct {
    // The file descriptor for the socket
    int connectionFd;
    // A pointer to a bank data structure
    bankType * bankData;
    // A pointer to a locks structure
    locksType * dataLocks;
} threadDataType;

// User to contain data after withrawing
typedef struct withdrawStatusStruct{
    float balance;
    int error;
} withdrawStatusType;

//Struct used to pass connection data to threads
typedef struct conenctionData{
    locksType * dataLocks;
    bankType * bankData;
    int fd;
} connectionDataType;

//Struct used to contain incomming messages
typedef struct incommingStruct{
    int account;
    float value;
    operationTransaction op;
} incommingMessageType;
//Function Declaration
void parseIncomming(incommingMessageType* output, char* s);
void sendResponse(int connectionFd, responseTransaction code, float balance);
void usage(char * program);
void setupHandlers();
void freeBankMemory(bankType * bankData, locksType * dataLocks);
float getBalance(connectionDataType* data, int account_num);
float deposit(connectionDataType* data, int account_num, float ammount);
void withdraw(withdrawStatusType* status, connectionDataType* data, int account_num, float amount);
int threadTransactions(connectionDataType* data);
int numberOfTransacrions(bankType* bankData, pthread_mutex_t* trans_lock);
void exitHandler(int signal);
void initBank(bankType * bankData, locksType * dataLocks);
void waitForConnections(int server_fd, bankType * bankData, locksType * dataLocks);
void * attentionThread(void * arg);
int accountValidation(int account);
#endif
