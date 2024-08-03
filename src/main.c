#include <stdio.h>

#include "../include/vm.h"

Instruction program[] = {
    PUSH_INSTRUCTION(0), //0
    PUSH_INSTRUCTION(1), //1
    PLUS_INSTRUCTION,    //2
    JUMP_INSTRUCTION(1), //3
    HALT_INSTRUCTION,    //4
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