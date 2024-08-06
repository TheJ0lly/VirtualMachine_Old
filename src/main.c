#include <stdio.h>

#include "../include/vm.h"



int main() {
    VM vm = vm_create_new(TEST_STACK_SIZE);
    Instruction program[] = {
        MOVE_INST(0, &vm.registers.RA),
        MOVE_INST(1, &vm.registers.RB),
        MOVE_INST(1, &vm.registers.RD),
        UNLOAD_INST(&vm.registers.RA),
        UNLOAD_INST(&vm.registers.RB),
        LOAD_INST(&vm.registers.RA),
        APPEND_INST(&vm.registers.RB),
        CMP_VAL(4, &vm.registers.RD),   // Change value here to make it loop a certain number of times. 0 <= - will be infinite.
        PUSH_INST(1),
        APPEND_INST(&vm.registers.RD),
        JNE_INST(3),
        RETURN_INST(&vm.registers.RB),
        HALT_INST,
    };


    int64_t out = 0;
    Error err = vm_execute_program(&vm, program, &out);
    
    if (err != ERR_OK) {
        printf("ERROR: %s\n", error_as_string(err));
        vm_dump_registers(&vm, stdout);
        vm_dump_stack(&vm, stdout);
    } else {
        printf("Returned value from Fibonacci: %ld\n", out);
    }



}