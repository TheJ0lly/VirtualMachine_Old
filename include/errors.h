#ifndef ERRORS_H_
#define ERRORS_H_

typedef enum {
    ERR_OK = 0,
    
    // Jump errors
    ERR_INVALID_JUMP_ADDRESS,
    ERR_INVALID_MEMORY_ACCESS,

    // Stack errors
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_EMPTY,
    ERR_STACK_FREE_UNALLOCED,
    ERR_STACK_MEMORY_OVERFLOW,

    ERR_UNKNOWN_OPERATION,
} Error;

const char *error_as_string(Error err);

#endif // ERRORS_H_