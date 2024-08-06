#ifndef VM_H_
#define VM_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TEST_STACK_SIZE 1024

#define PUSH_INST(x) {.op=PUSH, .value=(x)}
#define PEEK_INST {.op=PEEK}
#define RETURN_INST(addr) {.op=RETURN, .reg=(addr)}
#define POP_INST {.op=POP}
#define PLUS_INST {.op=PLUS}
#define MINUS_INST {.op=MINUS}
#define DIV_INST {.op=DIV}
#define MULT_INST {.op=MULT}
#define HALT_INST {.op=HALT}
#define LOAD_INST(addr) {.op=LOAD, .reg=(addr)}
#define UNLOAD_INST(addr) {.op=UNLOAD, .reg=(addr)}
#define JUMP_INST(x) {.op=JUMP, .value=(x)}
#define JUMPEQ_INST(x, addr) {.op=JUMPEQ, .value=(x), .reg=(addr)}

#define PROGRAM_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


typedef enum {
    PUSH = 0,
    PEEK,
    RETURN,
    POP,
    HALT,
    PLUS,
    MINUS,
    DIV,
    MULT,
    LOAD,
    UNLOAD,
    JUMP,
    JUMPEQ,
} Operation;

typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_EMPTY,
    ERR_PROGRAM_WITHOUT_HALT,
    ERR_INVALID_JUMP_ADDRESS,
    ERR_INVALID_MEMORY_ACCESS,
    ERR_UNKNOWN_OPERATION,
} Error;

// Instruction is a struct that holds the data needed to perform an instruction on the VM.
typedef struct Instruction {
    //op is the type of operation to be performed.
    Operation op;

    //value is a placeholder for data to be used in an operation.
    int64_t value;

    //reg is a pointer to the register that holds a value that will be compared in an operation.
    int64_t *reg;
} Instruction;

typedef struct Registers {
    int64_t RA;
    int64_t RB;
    int64_t RC;
} Registers;


typedef struct VM {
    //stack is the actual data container.
    int64_t *stack;

    //size is the current size of the elements in the stack.
    uint64_t size;

    //cap is the capacity of the stack.
    uint64_t cap;

    //ip is the instruction pointer, which points to the next instruction to be executed.
    uint64_t ip;

    //halt is a boolean that indicates that a program is running on the VM.
    bool halt;
    Registers registers;
} VM;

/*
vm_create_new creates a new instance of a VM.

capacity: the max number of elements(int64_t) that this instance will store.
*/
VM vm_create_new(size_t capacity);

//vm_free_memory frees the memory of the stack.
void vm_free_memory(VM *instance);

/*
vm_execute_program executes a program. A program is a set of instructions.

instance:     the VM instance which will run the instructions.
instructions: the set of instructions to be run / the program.
size:         the size of the program to be run. This is mostly to check for the HALT instruction in a program.
*/
Error vm_execute_program(VM *instance, Instruction *instructions, int size, int64_t *out);
Error vm_execute_instruction(VM *instance, Instruction instruction, int64_t *out);
const char *error_as_string(Error err);

void vm_dump_stack(VM *instance, FILE *stream);
void vm_dump_registers(VM *instance, FILE *stream);


#endif //VM_H_