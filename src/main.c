#include <stdio.h>

#include "../include/vm.h"

int main() {
    VM vm = vm_create_new(TEST_STACK_SIZE);
    
    int64_t out = 0;
    Error err = vm_execute_program(&vm, Fibonacci_Test(&vm, 5), &out);
    
    if (err != ERR_OK) {
         printf("ERROR: %s\n", error_as_string(err));
         vm_dump_registers(&vm, stdout);
         vm_dump_stack(&vm, stdout);
    } else {
         printf("Returned value from Fibonacci: %ld\n", out);
    }
}
