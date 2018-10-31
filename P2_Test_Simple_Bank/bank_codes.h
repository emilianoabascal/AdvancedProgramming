/*
    Emiliano Abascal Gurria A01023234
    21/10/2018
*/

#ifndef BANK_CODES_H
#define BANK_CODES_H

// The different types of operations available
typedef enum valid_operations {CHECK,DEPOSIT,WITHDRAW,EXIT} operationTransaction;
// The types of responses available
typedef enum valid_responses {OK,INSUFFICIENT,NO_ACCOUNT,BYE,ERROR} responseTransaction;
#endif
