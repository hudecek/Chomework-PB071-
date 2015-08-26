#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char** line;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedList {
    Node *first;
    Node *last;
    int* max;
} LinkedList;

void initList(LinkedList* l) {
    l->first = NULL;
    l->last = NULL;
    l->max = NULL;
}

Node *insert(LinkedList *list, char** value) {
    Node *newNode = malloc(sizeof(Node));
    newNode->line=value;

    if(list == NULL ) {
        return NULL;
    }
    if(list->first==NULL) {
        newNode->next = NULL;
        newNode->prev = NULL;
        list->first = newNode;
        list->last = newNode;

    } else {

        list->last->next = newNode;
        newNode->prev = list->last;
        list->last = newNode;

    }
    return newNode;
}
/*
void delete(LinkedList *list, Node *node) {
    //TODO
    if(list->first != NULL) {
        if(list->first == node ) {
            list->first = node->next;
        } else if(list->last == node) {
            list->last = node->prev;
            list->last->next = NULL;
        } else {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    }
    free(node);
}

*/

unsigned* getBiggest(LinkedList list, int cols) {
    unsigned* max = calloc(0, cols*sizeof(int));
    Node* walker = list.first;
    while(walker!=NULL) {
        for(int i=0; i<cols; i++) {
            if(max[i] < strlen(walker->line[i]))
                max[i] = strlen(walker->line[i]);
        }
        walker = walker->next;
    }
    return max;
}

void printAll(LinkedList list, int num, int noCols) {
    Node* walker = list.first;
    unsigned* maximums = getBiggest(list, noCols);
    while(walker!=NULL) {
        for(int i=0; i < noCols; i++) {
            printf("%s\t", walker->line[i]);
            for(unsigned i = strlen(walker->line[i]); i<maximums[i]; i++) {
                printf(" ");
            }
        }
        printf("\n");
        walker = walker->next;
    }
}

int scanAll(FILE* f, LinkedList* data, int* cols) {
    char c = fgetc(f);
    char** line = NULL;
    int length = 0;
    int no = 0;
    data->max = malloc(sizeof(int));

    while(!feof(f)) {

        line = realloc(line, ((*cols)+1)*sizeof(char*));
        *cols = 0;
        while(c!=',' && c!='\n') {
            line[*cols] = realloc(line[*cols], length+1);
            line[*cols][length] = c;
            c = fgetc(f);
            length++;
        }
        line[*cols] = realloc(line[*cols], length+1);
        line[*cols][length] = '\0';
        (*cols)++;
        if(c == '\n') {
            insert(data, line);
            line = NULL;
            no++;
        }
         length = 0;
         c = fgetc(f);
    }
    return no;
}



int main(int argc, char** argv)
{
    LinkedList data;
    initList(&data);
    FILE* f = NULL;
    int no = 0;
    int cols = 0;

    if(argc < 2) {
        fprintf(stderr, "Not enough arguments supplied!");
        return 1;
    }
    if((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Unable to open file");
        return 2;
    }

    no = scanAll(f, &data, &cols);
    printAll(data, no, cols);


    return 0;
}

