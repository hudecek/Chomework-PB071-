#define STDIO_H
#include <stdio.h>
#define STDLIB_H
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "queue.h"
#include <limits.h> //INT_MAX

/**
 * @brief removeEnter - substitutes the new line character in a string with ending character ('\0')
 * @param str - string to remove NL char from
 */
void removeEnter(char* str) {
    if(str!=NULL) {
        char* x;
        x = strchr(str, '\n');
        if(x)
            *x = '\0';
    }
}

/**
 * @brief freeAll - frees allocated memory
 * @param queue - dynamically allocated queue
 * @param instArr - dynamically allocated array containing pointers to instructions
 */
void freeAll(struct queue* queue, struct instruction** instArr) {
    queue_clear(queue);
    free(instArr);
}

/**
 * @brief runInstruction - carries out the instruction
 * @param mem_register
 * @param inst - instruction to carry out
 */

void runInstruction(int* mem_register, struct instruction* inst) {
    switch(inst->type) {
    case INST_ADD:
        *mem_register += inst->arg;
        break;
    case INST_SUB:
        *mem_register -= inst->arg;
        break;
    case INST_INC:
        ++*mem_register;
        break;
    case INST_DEC:
        --*mem_register;
        break;
    case INST_MUL:
        *mem_register *= inst->arg;
        break;
    case INST_DIV:
        *mem_register /= inst->arg;
        break;
    }
    free(inst);
}

/**
 * @brief run - ensures the correct number of instruction is processed by runInstruction function
 * @param numToRun - number of instruction that should be run
 * @param mem_register
 * @param queue
 */
void run(int numToRun, int *mem_register, struct queue* queue) {\
    struct instruction* instr;
    int over=0;
    int i=0;
    while(!over) {
        if(queue_empty(queue) || numToRun == i)
            over = 1;
        else {
            instr = queue_pop(queue);
            runInstruction(mem_register, instr);
            i++;
        }
    }
}

/**
 * @brief compareStrings - compares the string with an array of options
 * @param count - number of options in an array
 * @param strings - array of options
 * @param string - string to compare
 * @return - index of option that is represented by string, if there is no such an option in an array returns -1
 */

int compareStrings(int count, char** strings, char* string) {
    for(int i=0; i<count; i++) {
        if(strcmp(strings[i], string)==0)
            return i;
    }
    return -1;
}

/**
 * @brief strToNum - converts string to number
 * @param string - string that contains a number
 * @return - value of number found in a string, 0 if NULL is passed or no number was found
 */
int strToNum(char* string) {
    if(string == NULL)
        return 0;
    int result= atoi(string);
    return result;
}

/**
 * @brief numericOperation - assigns an instruction type corresponding to a given number
 * @param instNum - number of an operation
 * @param inst - instruction to assign type to
 */
void numericOperation(int instNum, struct instruction* inst) {
    switch(instNum) {
    case 1:
        inst->type = INST_ADD;
        break;
    case 2:
        inst->type = INST_SUB;
        break;
    case 3:
        inst->type = INST_INC;
        break;
    case 4:
        inst->type = INST_DEC;
        break;
    case 5:
        inst->type = INST_MUL;
        break;
    case 6:
        inst->type = INST_DIV;
        break;
    }
}

/**
 * @brief consoleMode - communicates with user
 * @param mem_register
 * @param stack
 * @param queue
 * @param numRun - number of instruction to be evaluated on 'run' command
 */
void consoleMode(int *mem_register,
                 struct stack* stack, struct queue* queue, int numRun) {
    int quit = 0;
    int choice = -1;
    int i = 0; //instruction counter
    int numArg = 0; //numeric argument
    char* options[] = {"run", "add", "sub", "inc", "dec", "mul", "div", "push",
                       "pop", "quit", "reset"};
    char task[20];
    char* arg;
    char* argVal;
    struct instruction** instArr;
    instArr = malloc(10*sizeof(struct instruction*));

    while(!quit) {
        if(i % 10 == 9) //allocate additional memory for instruction array
            instArr = realloc(instArr, (i+11)*sizeof(struct instruction*));
        printf("> ");
        fgets(task, 20, stdin);
        arg = strtok(task, " ");
        argVal = strtok(NULL, " ");
        removeEnter(arg);
        removeEnter(argVal);

        numArg = strToNum(argVal);
        choice = compareStrings(11, options, arg); // sth else
        switch(choice) {
        case 0:
            run(numRun, mem_register, queue);
            print(stack);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            instArr[i] = malloc(sizeof(struct instruction));
            instArr[i]->arg = numArg;
            instArr[i]->next = NULL;
            numericOperation(choice, instArr[i]);
            queue_push(queue, *(instArr+i));
            i++;
            break;
        case 7:
            stack_push(stack);
            break;
        case 8:
            stack_pop(stack);
            break;
        case 9:
            quit = 1;
            freeAll(queue, instArr);
            break;
        case 10:
            queue_clear(queue);
            break;
        }
    }

}

/**
 * @brief printHelp - prints help
 */
void printHelp() {
    printf("Use: <PATH_TO_PROGRAM> <ARGUMENT>\n"
           "Acts as a simple processor with instructions stored in stack.\n"
           "Exactly one of arguments has to be passed.\n"
           "Arguments:\n\n"
           "\t-r <NUMBER>\tSets the number of instructions executed after\n"
           "\t\t\t'run' command to NUMBER\n"
           "\t-R\t\tExecutes all instructions after 'run' command\n"
           "\t-h\t\tPrints this help file\n\n\n"
           "In case of incorrect number of arguments passed, the program ends.\n"
           "Available instructions:\n\n"
           "\tadd <NUMBER>\tIncreases the value of register by NUMBER\n"
           "\tsub <NUMBER>\tSubtracts the NUMBER from value of register\n"
           "\tinc\t\tIncreases the value of register by 1\n"
           "\tdec\t\tDecreases the value of register by 1\n"
           "\tmul\t\tMultiplies the value of register by NUMBER\n"
           "\tdiv\t\tDivides the value of register by NUMBER\n\n"
           "Options:\n\n"
           "\trun\t\tRuns NUMBER of instructions passed\n"
           "\treset\t\tRemoves all values from the stack\n"
           "\tpusht\t\tPushes the value of register to the stack\n"
           "\tpop\t\tRemoves the last value from the stack\n"
           "\tquit\t\tEnds the program\n\n"
           "Program ends when instruction 'quit' is passed.\n");
}

int main(int argc, char** argv)
{
    my_assert(1 < argc && argc <= 3);
    int numRun;

    struct queue queue;
    queue_create(&queue);
    struct stack stack;
    stack.stack_top = 0;
    extern int mem_register;

    if(strcmp(argv[1], "-r") == 0) {
        my_assert(argc == 3);
        numRun = atoi(argv[2]);
        my_assert(numRun > 0);
        consoleMode(&mem_register, &stack, &queue, numRun);
    } else if(strcmp(argv[1], "-R") == 0){
        my_assert(argc == 2);
        numRun = INT_MAX;
        consoleMode(&mem_register, &stack, &queue, numRun);
    } else if(strcmp(argv[1], "-h") == 0) {
        my_assert(argc == 2);
        printHelp();
    } else {
        fprintf(stderr,"Error: Incorrect argument!\n");
    }
    return 0;
}

