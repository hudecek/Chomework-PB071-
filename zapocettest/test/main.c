#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct person {
    char* name;
    char* surename;
    int age;
} person;

void alternPerson(person* p) {
    if(p!=NULL) {
        for(unsigned i=0; i< strlen(p->name); i++) {
            p->name[i] = toupper(p->name[i]);
        }
        for(unsigned i=0; i< strlen(p->surename); i++) {
            p->surename[i] = toupper(p->surename[i]);
        }
        p->age++;
    } else {
        fprintf(stderr, "Null parameter supplied to function alternPerson");
    }
}

char* readSth (FILE* f) {
    if(f!=NULL) {
        char c = 0;
        int size = 0;
        char* string = NULL;
        char* tmp = NULL;
        while(c!=':' && c!='\n' && !feof(f)) {
            fscanf(f, "%c", &c);
            tmp = realloc(string, size+1);
            if(tmp!=NULL)
                string = tmp;
            else {
                fprintf(stderr, "Problem while allocating memory");
                free(string);
                return NULL;
            }
            string[size] = c;
            size++;
        }
        string[size-1] = '\0';
        return string;
    } else {
        fprintf(stderr, "Null parameter supplied to function readSth");
        return NULL;
    }
}

void printPerson(person* p, FILE* f) {
    if(p!=NULL && f!=NULL) {
        fprintf(f, "%s:%s:%d\n", p->name, p->surename, p->age);
        printf("%s:%s:%d\n", p->name, p->surename, p->age);
    } else {
        fprintf(stderr, "Null parameter suppled to function printPerson");
    }
}

void freeAll(person* arr) {
    if(arr!=NULL) {
        free(arr->name);
        free(arr->surename);
        arr->name = arr->surename = NULL;
    } else {
        fprintf(stderr, "Null parameter supplied to function freeAll");
    }
}

int main(int argc, char** argv)
{
    if(argc < 3) {
        fprintf(stderr, "Too few parameters supplied!");
        return 1;
    }
    int no = atoi(argv[2]);
    person a[no];
    FILE* f = fopen(argv[1], "r");
    rewind(f);
    for(int i=0; i< no; i++) {
        a[i].name = readSth(f);
        if(a[i].name == NULL)
            return 2;
        a[i].surename = readSth(f);
        if(a[i].surename == NULL)
            return 2;
        fscanf(f, "%d\n", &a[i].age);
    }
    fclose(f);

    char* nN = malloc(strlen(argv[1]+5));
    if(nN == NULL) {
        fprintf(stderr, "Problem while allocating memory.");
        return 2;
    }
    sprintf(nN, "%s.tmp", argv[1]);
    FILE* nf = fopen(nN, "w");

    for(int i=0; i<no; i++) {
        alternPerson(&a[i]);
        printPerson(&a[i],nf);
        freeAll(&a[i]);
    }
    fclose(nf);

    free(nN);
    return 0;
}

