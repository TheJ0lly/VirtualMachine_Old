#include "../include/errors.h"

const char *error_as_string(Error err) {
    switch (err) {
    case ERR_OK:
        return "ERR_OK";

    case ERR_INVALID_JUMP_ADDRESS:
        return "ERR_INVALID_JUMP_ADDRESS";
    case ERR_INVALID_MEMORY_ACCESS:
        return "ERR_INVALID_MEMORY_ACCESS";

    case ERR_STACK_OVERFLOW:
        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
        return "ERR_STACK_UNDERFLOW";
    case ERR_STACK_EMPTY:
        return "ERR_STACK_EMPTY";
    case ERR_STACK_FREE_UNALLOCED:
        return "ERR_STACK_FREE_UNALLOCED";
    case ERR_STACK_MEMORY_OVERFLOW:
        return "ERR_STACK_MEMORY_OVERFLOW";

    default:
        return "ERR_UNKNOWN_OPERATION";
    }
}