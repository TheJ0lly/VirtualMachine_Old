#include <stdio.h>

#include "../include/vm.h"



int main() {
    VM vm = vm_create_new(TEST_STACK_SIZE);

    Instruction program[] = {
        PUSH_INST(2),                      //0
        PUSH_INST(3),                      //1
        PLUS_INST,                         //2
        LOAD_INST(&vm.registers.RA),       //3
        PUSH_INST(5),                      //4
        JUMPEQ_INST(1, &vm.registers.RA),  //5
        UNLOAD_INST(&vm.registers.RA),     //6
        PLUS_INST,                         //7
        PEEK_INST,                         //8
        HALT_INST,                         //9
    };


    int64_t out = 0;
    Error err = vm_execute_program(&vm, program, PROGRAM_SIZE(program), &out);
    
    if (err != ERR_OK) {
        printf("ERROR: %s\n", error_as_string(err));
        vm_dump_registers(&vm, stdout);
        vm_dump_stack(&vm, stdout);
    } else {
        printf("Returned value: %ld\n", out);
    }



}