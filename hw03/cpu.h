#ifndef CPU_H
#define CPU_H

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif //stdio_h

struct stack {
    unsigned int stack_top;
    int stack_values[20];
};

int mem_register;

void stack_initialize(struct stack* stack);
void stack_push (struct stack* stack);
void stack_pop (struct stack* stack);
void print (const struct stack* stack);

#endif //cpu_h
