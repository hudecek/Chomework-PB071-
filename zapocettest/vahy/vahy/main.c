#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct preson {
    char* name;
    int weigth;
} person;

void getInfo(FILE* f, person* arr, int no) {
    char c = 0;
    int count = 0;
    for(int i=0; i<no; i++) {
        c = fgetc(f);
        count = 0;
        while(c!=':') {
            arr[i].name = realloc(arr[i].name,count + 1);
            arr[i].name[count] = c;
            count++;
            c = fgetc(f);
        }
        arr[i].name = realloc(arr[i].name,count + 1);
        arr[i].name[count] = '\0';
        fscanf(f, "%d\n", &arr[i].weigth);
    }
}

int countWeight(person* arr, int no) {
    int w = 0;
    for(int i=0; i<no; i++) {
        w += arr[i].weigth;
    }
    return w;
}

void freeArray(person* array, int no) {
    for(int i=0; i < no; i++) {
        free(array->name);
        array->name = NULL;
    }
}

void initializeArr(person* array, int no) {
    for(int i=0; i<no; i++)
        array[i].name = NULL;
}

int main(int argc, char** argv)
{
    if(argc < 3) {
        fprintf(stderr,"Not enough arguments supplied!");
        return 1;
    }

    int no = atoi(argv[2]);
    char* fname = argv[1];
    person array[no];
    initializeArr(array, no);
    FILE* f = fopen(fname, "r");
    if(f == NULL) {
        fprintf(stderr, "problem while openiong file");
        return 2;
    }

    getInfo(f, array, no);

    printf("First name: %s\nTotal weight: %d\n", array[0].name, countWeight(array, no));

    freeArray(array, no);

    return 0;
}

