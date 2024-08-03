#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TEST_STACK_SIZE 1024

#define PUSH_INSTRUCTION(x) {.op=PUSH, .value=(x)}
#define PEEK_INSTRUCTION {.op=PEEK}
#define POP_INSTRUCTION {.op=POP}
#define PLUS_INSTRUCTION {.op=PLUS}
#define MINUS_INSTRUCTION {.op=MINUS}
#define DIV_INSTRUCTION {.op=DIV}
#define MULT_INSTRUCTION {.op=MULT}
#define HALT_INSTRUCTION {.op=HALT}
#define JUMP_INSTRUCTION(x) {.op=JUMP, .value=(x)}

#define PROGRAM_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

typedef enum {
    PUSH = 0,
    PEEK,
    POP,
    HALT,
    PLUS,
    MINUS,
    DIV,
    MULT,
    JUMP,
} Operation;

typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_EMPTY,
    ERR_PROGRAM_WITHOUT_HALT,
    ERR_UNKNOWN_OPERATION,
    ERR_INVALID_JUMP_ADDRESS,
} Error;

typedef struct Instruction {
    Operation op;
    int64_t value;
} Instruction;

typedef struct VM {
    int64_t *stack;
    uint64_t size;
    uint64_t cap;
    uint64_t ip;
    bool halt;
} VM;

VM vm_create_new(size_t capacity);
void vm_free_memory(VM *instance);

Error vm_execute_program(VM *instance, Instruction *instructions, int size, int64_t *out);
Error vm_execute_instruction(VM *instance, Instruction instruction, int64_t *out);
const char *error_as_string(Error err);

void vm_dump_stack(VM *instance, FILE *stream);


#endif //STACK_H_