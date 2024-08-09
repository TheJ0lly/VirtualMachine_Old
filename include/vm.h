#ifndef VM_H_
#define VM_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define TEST_STACK_SIZE 1024
#define DEBUG


#define LABEL_INST(name) {.op=LABEL}

//PUSH_INST pushses the specified value on top of the memory stack.
#define PUSH_INST(x) {.op=PUSH, .value=(x)}

//MOVE_INST put the value x into the register with the address of addr.
#define MOVE_INST(x, addr) {.op=MOVE, .value=(x), .primary_reg=(addr)}

//PEEK_INST will store the value on top of the stack inside the out pointer passed into the vm_execute_program.
#define PEEK_INST {.op=PEEK}

//RETURN_INST will store the value from a register inside the out pointer passed into the vm_execute_program.
#define RETURN_INST(addr) {.op=RETURN, .primary_reg=(addr)}

//POP_INST will pop the element on top of the stack.
#define POP_INST {.op=POP}

//PLUS_INST will perform the plus operation between the last 2 elements on the stack.
#define PLUS_INST {.op=PLUS}

//APPEND_INST will add the value on top of the stack into a register.
#define APPEND_INST(addr) {.op=APPEND, .primary_reg=(addr)}

//APPEND_VAL_INST will add the value x into a register.
#define APPEND_VAL_INST(x, addr) {.op=APPEND_VAL, .value=(x), .primary_reg=(addr)}

//MINUS_INST will perform the minus operation between the last 2 elements on the stack.
#define MINUS_INST {.op=MINUS}

//DECREMENT_INST will decrease the value from a register by the last value on top of the stack.
#define DECREMENT_INST(addr) {.op=DECREMENT, .primary_reg=(addr)}

//DECREMENT_INST will decrease the value from a register by x.
#define DECREMENT_VAL_INST(x, addr) {.op=DECREMENT_VAL, .value=(x), .primary_reg=(addr)}

//DIV_INST will perform the division operation between the last 2 elements on the stack.
#define DIV_INST {.op=DIV}

//MULT_INST will perform the multiplication operation between the last 2 elements on the stack.
#define MULT_INST {.op=MULT}

//HALT_INST will stop the execution of the VM.
#define HALT_INST {.op=HALT}

//LOAD_INST will put the value on top of the stack into a register.
#define LOAD_INST(addr) {.op=LOAD, .primary_reg=(addr)}

//UNLOAD_INST will put the value from a register on top of the stack.
#define UNLOAD_INST(addr) {.op=UNLOAD, .primary_reg=(addr)}

//CMP compares the values of 2 registers and then sets the compare_flag register to either -1, 0, 1.
#define CMP(addr1, addr2) {.op=CMP, .primary_reg=(addr1), .secondary_reg=(addr2)}

//CMP_VAL compares the values of a register and a arbitrary value then sets the compare_flag register to either -1, 0, 1.
#define CMP_VAL(x, addr) {.op=CMP_VAL, .value=(x), .primary_reg=(addr)}

//JUMP_INST will jump to an instruction in the program.
#define JUMP_INST(x) {.op=JUMP, .value=(x)}

//JEQ_INST will jump to an instruction in the program if the compare_flag register is 0.
#define JEQ_INST(x) {.op=JEQ, .value=(x)}

//JNE_INST will jump to an instruction in the program if the compare_flag register is not 0.
#define JNE_INST(x) {.op=JNE, .value=(x)}

//JGR_INST will jump to an instruction in the program if the compare_flag register is 1.
#define JGR_INST(x) {.op=JGR, .value=(x)}

//JNE_INST will jump to an instruction in the program if the compare_flag register is 0 or 1.
#define JGE_INST(x) {.op=JGE, .value=(x)}

//JNE_INST will jump to an instruction in the program if the compare_flag register is 0 or -1.
#define JLE_INST(x) {.op=JLE, .value=(x)}

//JNE_INST will jump to an instruction in the program if the compare_flag register is -1.
#define JLS_INST(x) {.op=JLS, .value=(x)}


typedef enum {
    LABEL = 0,
    PUSH,
    MOVE,
    PEEK,
    RETURN,
    POP,
    HALT,
    PLUS,
    APPEND,
    APPEND_VAL,
    MINUS,
    DECREMENT,
    DECREMENT_VAL,
    DIV,
    MULT,
    LOAD,
    UNLOAD,
    CMP,
    CMP_VAL,
    JUMP,
    JEQ,
    JNE,
    JGR,
    JGE,
    JLE,
    JLS,
} Operation;

typedef enum {
    ERR_OK = 0,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_EMPTY,
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

    //primary_reg is a pointer to the register that holds a value that will be compared in an operation.
    //If using a single register, always use this first.
    int64_t *primary_reg;

    //secondary_reg is a pointer to the register that holds a value that will be compared in an operation.
    int64_t *secondary_reg;
} Instruction;

typedef struct Registers {
    int64_t *RA;
    int64_t *RB;
    int64_t *RC;
    int64_t *RD;
    int32_t *RE;
    int32_t *RF;
    int32_t *RG;
    int32_t *RH;

    //compare_flag can have three values:
    //-1 - The left operand is bigger.
    //0  - Both operands are equal.
    //1  - The right operand is bigger.
    int8_t compare_flag;
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
Error vm_execute_program(VM *instance, Instruction *instructions, int64_t *out);
Error vm_execute_instruction(VM *instance, Instruction instruction, int64_t *out);
const char *error_as_string(Error err);
const char *operation_as_string(Operation op);

Instruction *Fibonacci_Test(VM *vm, int iter);

void vm_dump_stack(VM *instance, FILE *stream);
void vm_dump_registers(VM *instance, FILE *stream);


#endif //VM_H_