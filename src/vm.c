#include "../include/vm.h"

VM vm_create_new(size_t capacity) {
    VM vm;
    vm.cap = capacity;
    vm.size = 0;
    vm.ip = 0;
    vm.stack = (int64_t *)malloc(sizeof(int64_t) * capacity);
    vm.halt = false;

    return vm;
}

void vm_free_memory(VM *instance) {
    instance->cap = 0;
    instance->ip = 0;
    instance->size = 0;
    free(instance->stack);
}

Error vm_execute_program(VM *instance, Instruction *instructions, int64_t *out) {
    instance->ip = 0;

    while (!instance->halt) {
        Error err = vm_execute_instruction(instance, instructions[instance->ip], out);
        if (err != ERR_OK) {
            return err;
        }
    }

    return ERR_OK;

}

Error vm_execute_instruction(VM *instance, Instruction instruction, int64_t *out) {
    switch (instruction.op)
    {
    case PUSH:
        if (instance->size + 1 > instance -> cap) {
            return ERR_STACK_OVERFLOW;
        }

        instance->stack[instance->size++] = instruction.value;
        instance->ip++;
        return ERR_OK;

    case MOVE:
        *instruction.primary_reg = instruction.value;
        instance->ip++;
        return ERR_OK;

    case PEEK:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }

        if (out != NULL) {
            *out = instance->stack[instance->size-1];
        }
        instance->ip++;
        return ERR_OK;

    case RETURN:
        *out = *instruction.primary_reg;
        instance->ip++;
        return ERR_OK;
    
    case POP:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }
        instance->size--;
        instance->ip++;
        return ERR_OK;

    case HALT:
        instance->halt = true;
        return ERR_OK;

    case PLUS:
        if (instance->size < 2) {
            return ERR_STACK_UNDERFLOW;
        }

        instance->stack[instance->size-2] += instance->stack[instance->size-1];
        instance->size--;
        instance->ip++;
        return ERR_OK;

    case APPEND:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }

        *instruction.primary_reg += instance->stack[instance->size-1];
        instance->size--;
        instance->ip++;
        return ERR_OK;

    case APPEND_VAL:
        *instruction.primary_reg += instruction.value;
        instance->ip++;
        return ERR_OK;

    case MINUS:
        if (instance->size < 2) {
            return ERR_STACK_UNDERFLOW;
        }

        instance->stack[instance->size-2] -= instance->stack[instance->size-1];
        instance->size--;
        return ERR_OK;

    case DECREMENT:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }

        *instruction.primary_reg -= instance->stack[instance->size-1];
        instance->size--;
        instance->ip++;
        return ERR_OK;

    case DECREMENT_VAL:
        *instruction.primary_reg -= instruction.value;
        instance->ip++;
        return ERR_OK;
    
    case DIV:
        if (instance->size < 2) {
            return ERR_STACK_UNDERFLOW;
        }

        instance->stack[instance->size-2] /= instance->stack[instance->size-1];
        instance->size--;
        instance->ip++;
        return ERR_OK;

    case MULT:
        if (instance->size < 2) {
            return ERR_STACK_UNDERFLOW;
        }

        instance->stack[instance->size-2] *= instance->stack[instance->size-1];
        instance->size--;
        instance->ip++;
        return ERR_OK;
    
    case LOAD:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }

        *instruction.primary_reg = instance->stack[instance->size-1];
        instance->size--;
        instance->ip++;
        return ERR_OK;

    case UNLOAD:
        instance->stack[instance->size++] = *instruction.primary_reg;
        instance->ip++;
        return ERR_OK;

    case CMP:
        if (*instruction.primary_reg > *instruction.secondary_reg) {
            instance->registers.compare_flag = -1;
            goto return_from_cmp;
        }
        
        if (*instruction.primary_reg < *instruction.secondary_reg) {
            instance->registers.compare_flag = 1;
            goto return_from_cmp;
        }

        instance->registers.compare_flag = 0;
        
        return_from_cmp:
        instance->ip++;
        return ERR_OK;

    case CMP_VAL:
        if (*instruction.primary_reg > instruction.value) {
            instance->registers.compare_flag = 1;
            goto return_from_cmp_val;
        }
        
        if (*instruction.primary_reg < instruction.value) {
            instance->registers.compare_flag = -1;
            goto return_from_cmp_val;
        }

        instance->registers.compare_flag = 0;
        
        return_from_cmp_val:
        instance->ip++;
        return ERR_OK;

    case JUMP:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }

        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        instance->ip = instruction.value;
        return ERR_OK;
    
    case JEQ:
        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        if (instance->registers.compare_flag == 0) {
            instance->ip = instruction.value;
        } else {
            instance->ip++;
        }

        return ERR_OK;
    
    case JNE:
        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        if (instance->registers.compare_flag != 0) {
            instance->ip = instruction.value;
        } else {
            instance->ip++;
        }

        return ERR_OK;

    case JGR:
        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        if (instance->registers.compare_flag == 1) {
            instance->ip = instruction.value;
        } else {
            instance->ip++;
        }

        return ERR_OK;

    case JGE:
        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        if (instance->registers.compare_flag >= 0) {
            instance->ip = instruction.value;
        } else {
            instance->ip++;
        }

        return ERR_OK;

    case JLE:
        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        if (instance->registers.compare_flag <= 0) {
            instance->ip = instruction.value;
        } else {
            instance->ip++;
        }

        return ERR_OK;

    case JLS:
        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        if (instance->registers.compare_flag == -1) {
            instance->ip = instruction.value;
        } else {
            instance->ip++;
        }

        return ERR_OK;

    default:
        return ERR_UNKNOWN_OPERATION;
    }
}

const char *error_as_string(Error err) {
    switch (err) {
    case ERR_STACK_OVERFLOW:
        return "ERR_STACK_OVERFLOW";
    case ERR_STACK_UNDERFLOW:
        return "ERR_STACK_UNDERFLOW";
    case ERR_STACK_EMPTY:
        return "ERR_STACK_EMPTY";
    case ERR_INVALID_JUMP_ADDRESS:
        return "ERR_INVALID_JUMP_ADDRESS";
    default:
        return "ERR_UNKNOWN_OPERATION";
    }
}

const char *operation_as_string(Operation op) {
    switch (op)
    {
    case PUSH:
        return "PUSH";

    case MOVE:
        return "MOVE";

    case PEEK:
        return "PEEK";

    case RETURN:
        return "RETURN";
    case POP:
        return "POP";

    case HALT:
        return "HALT";

    case PLUS:
        return "PLUS";

    case APPEND:
        return "APPEND";

    case APPEND_VAL:
        return "APPEND_VAL";

    case MINUS:
        return "MINUS";
    
    case DIV:
        return "DIV";

    case MULT:
        return "MULT";
    
    case LOAD:
        return "LOAD";

    case UNLOAD:
        return "UNLOAD";

    case CMP:
        return "CMP";

    case CMP_VAL:
        return "CMP_VAL";

    case JUMP:
        return "JUMP";
    
    case JEQ:
        return "JEQ";

    case JNE:
        return "JNE";

    case JGE:
        return "JGE";
    
    case JGR:
        return "JGR";
    
    case JLE:
        return "JLE";

    case JLS:
        return "JLS";

    default:
        return "UNKNOWN_OPERATION";
    }
}


void vm_dump_stack(VM *instance, FILE *stream) {
    printf("Stack:\n");
    if (instance->size == 0) {
        printf("   [empty]\n");
    } else {
        for (int i = instance->size-1; i >= 0; i--) {
            printf("   %ld\n", instance->stack[i]);
        }
    }
}

void vm_dump_registers(VM *instance, FILE *stream) {
    printf("Registers:\n");

    printf("   RA: %ld\n", instance->registers.RA);
    printf("   RB: %ld\n", instance->registers.RB);
    printf("   RC: %ld\n", instance->registers.RC);
}


Instruction *Fibonacci_Test(VM *vm, int iter) {

    Instruction *Fibonacci = malloc(sizeof(Instruction) * 12);

    Fibonacci[0] = (Instruction)MOVE_INST(0, &vm->registers.RA);
    Fibonacci[1] = (Instruction)MOVE_INST(1, &vm->registers.RB);
    Fibonacci[2] = (Instruction)MOVE_INST(1, &vm->registers.RD);
    Fibonacci[3] = (Instruction)UNLOAD_INST(&vm->registers.RA);
    Fibonacci[4] = (Instruction)UNLOAD_INST(&vm->registers.RB);
    Fibonacci[5] = (Instruction)LOAD_INST(&vm->registers.RA);
    Fibonacci[6] = (Instruction)APPEND_INST(&vm->registers.RB);
    Fibonacci[7] = (Instruction)CMP_VAL(5, &vm->registers.RD);   // Change value here to make it loop a certain number of times. 0 <= - will be infinite.
    Fibonacci[8] = (Instruction)APPEND_VAL_INST(1, &vm->registers.RD);
    Fibonacci[9] = (Instruction)JNE_INST(3);
    Fibonacci[10] = (Instruction)RETURN_INST(&vm->registers.RB);
    Fibonacci[11] = (Instruction)HALT_INST;

    return Fibonacci;
}
