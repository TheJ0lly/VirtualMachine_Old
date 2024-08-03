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

static Error ensure_program_halt(Instruction *instructions, int size) {
    for (int i = 0; i < size; i++) {
        if (instructions[i].op == HALT) {
            return ERR_OK;
        }
    }

    return ERR_PROGRAM_WITHOUT_HALT;
}

Error vm_execute_program(VM *instance, Instruction *instructions, int size, int64_t *out) {
    Error err = ensure_program_halt(instructions, size);

    if (err != ERR_OK) {
        return err;
    }

    instance->ip = 0;

    while (!instance->halt) {
        if (instance->ip >= size) {
            return ERR_INVALID_MEMORY_ACCESS;
        }

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

    case PEEK:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }

        if (out != NULL) {
            *out = instance->stack[instance->size-1];
        }
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

    case MINUS:
        if (instance->size < 2) {
            return ERR_STACK_UNDERFLOW;
        }

        instance->stack[instance->size-2] -= instance->stack[instance->size-1];
        instance->size--;
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

        *instruction.reg = instance->stack[instance->size-1];
        instance->size--;
        instance->ip++;
        return ERR_OK;

    case UNLOAD:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }
        
        instance->stack[instance->size++] = *instruction.reg;
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
    
    case JUMPEQ:
        if (instance->size == 0) {
            return ERR_STACK_EMPTY;
        }

        if (instruction.value < 0 || instruction.value > instance->cap) {
            return ERR_INVALID_JUMP_ADDRESS;
        }

        if (instance->stack[instance->size-1] == *instruction.reg) {
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
    case ERR_PROGRAM_WITHOUT_HALT:
        return "ERR_PROGRAM_WITHOUT_HALT";
    case ERR_INVALID_JUMP_ADDRESS:
        return "ERR_INVALID_JUMP_ADDRESS";
    case ERR_INVALID_MEMORY_ACCESS:
        return "ERR_INVALID_MEMORY_ACCESS";
    default:
        return "ERR_UNKNOWN_OPERATION";
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


