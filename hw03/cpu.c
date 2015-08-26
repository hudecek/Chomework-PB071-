#include "cpu.h"

extern int mem_register;

#ifndef MEM_REGISTER_INITIALIZE
#define MEM_REGISTER_INITIALIZE
int mem_register = 0;
#endif // mem_register_initialize

/**
 * @brief stack_push - adds current register state to the stack
 * @param stack
 */
void stack_push (struct stack* stack) {
    if(stack == NULL) {
        fprintf(stderr, "Error: Passed argument NULL!\n");
    } else {
        if(stack->stack_top >= 20) {
            fprintf(stderr, "Error: Stack full!\n");
        } else {
            stack->stack_values[stack->stack_top] = mem_register;
            stack->stack_top += 1;
        }
    }

}

/**
 * @brief stack_pop - removes the most recent register state in the stack
 * @param stack
 */
void stack_pop (struct stack* stack) {
    if(stack == NULL) {
        fprintf(stderr, "Error: Passed argument NULL!\n");
    } else {
        if(stack->stack_top == 0) {
            fprintf(stderr, "Error: Stack empty!\n");
        } else {
            stack->stack_top -= 1;
            stack->stack_values[stack->stack_top] = 0;
        }
    }
}

/**
 * @brief print - prints the state of stack and register
 * @param stack
 */
void print (const struct stack* stack) {
    if(stack == NULL) {
        fprintf(stderr, "Error: Passed argument NULL!\n");
    } else {
        printf("# Register %d | Stack ", mem_register);
        for(unsigned int i=0; i < stack->stack_top; i++) {
            printf("%d ", stack->stack_values[i]);
        }
        printf("\n");
    }
}

