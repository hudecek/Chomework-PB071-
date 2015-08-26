#include "queue.h"
/**
 * @brief queue_create - initializes the queue (container)
 * @param container
 */
void queue_create (struct queue* container) {
    my_assert(container!=NULL);
    container->first = NULL;
    container->last = NULL;
}

/**
 * @brief queue_push - pushes an instruction to the end of the queue
 * @param container
 * @param item - instruction to be added to queue
 */
void queue_push (struct queue* container, struct instruction* item) {
    my_assert(container!=NULL);

    if(item == NULL) {
        fprintf(stderr, "Error: Argument 'item'not specified!\n");
    } else {
        if(container->first == NULL) {
            container->first = item;
        } else {
            container->last->next = item;
        }
        container->last = item;
    }
}

/**
 * @brief queue_pop - removes an instruction from queue
 * @param container
 * @return - pointer to an instruction if queue is not empty, NULL otherwise
 */
struct instruction* queue_pop (struct queue* container) {
    my_assert(container!=NULL);
    struct instruction* instr;
    if(container->first == NULL) {
        fprintf(stderr, "Error: The queue is empty!\n");
    } else {
        instr = container->first;
        if(container->first == container->last) {
            queue_create(container);
        } else {
            container->first = instr->next;
        }
        return instr;
    }
    return NULL;
}

/**
 * @brief queue_empty - finds out whether the queue is empty or not
 * @param container
 * @return 1 if empty, 0 if not
 */
unsigned int queue_empty (const struct queue* container) {
    my_assert(container!=NULL);
    if(container->first == NULL)
        return 1;
    return 0;
}

/**
 * @brief queue_size - determines the size of queue
 * @param container
 * @return - length of a queue
 */
unsigned int queue_size (const struct queue* container) {
    my_assert(container != NULL);
    int size=0;
    struct instruction* goThrough = container->first;
    while(goThrough != NULL) {
        size++;
        goThrough = goThrough->next;
    }
    return size;
}

/**
 * @brief queue_clear - removes all members of queue
 * @param container
 * @return number of instruction removed from queue
 */
unsigned int queue_clear (struct queue* container) {
    my_assert(container!=NULL);
    unsigned int removedCount=0;
    struct instruction* goThrough = container->first;
    struct instruction* toRemove;
    while(goThrough!=NULL) {
        toRemove = goThrough;
        goThrough = goThrough->next;
        free(toRemove);
        removedCount++;
    }
    queue_create(container);
    return removedCount;
}
