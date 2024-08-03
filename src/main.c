#include <stdio.h>

#include "../include/vm.h"

Instruction program[] = {
    PUSH_INSTRUCTION(2),
    PUSH_INSTRUCTION(3),
    PUSH_INSTRUCTION(4),
    PLUS_INSTRUCTION,
    MULT_INSTRUCTION,
    PUSH_INSTRUCTION(7),
    DIV_INSTRUCTION,
    PEEK_INSTRUCTION,
    HALT_INSTRUCTION,
};

int main() {
    VM vm = vm_create_new(TEST_STACK_SIZE);

    int64_t variable;
    Error err = vm_execute_program(&vm, program, PROGRAM_SIZE(program), &variable);

    if (err != ERR_OK) {
        printf("ERROR: %s\n", error_as_string(err));
    } else {
        printf("Returned variable: %ld\n", variable);
    }

}