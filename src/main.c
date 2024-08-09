#include <stdio.h>

#include "../include/vm.h"


int main() {
    VM vm = vm_create_new(TEST_STACK_SIZE);
    
    Instruction Program[] = {
        MOVE_INST(11, &vm.registers.RA),
        DECREMENT_VAL_INST(1, &vm.registers.RA),
        LABEL_INST(LOOP),
        CMP_VAL(10, &vm.registers.RA),
        JGE_INST(1),
        RETURN_INST(&vm.registers.RA),
        HALT_INST,
    };

    int64_t out = 0;
    Error err = vm_execute_program(&vm, Program, &out);
    
    if (err != ERR_OK) {
        printf("ERROR: %s\n", error_as_string(err));
        vm_dump_registers(&vm, stdout);
        vm_dump_stack(&vm, stdout);
    } else {
        printf("Returned value from Fibonacci: %ld\n", out);
    }



}