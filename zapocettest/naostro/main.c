#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Review {
    char* product;
    char trustworthy;
    int value;
} Review;
/*
typedef struct sum {
    float trustNum;
    float evaluation;
    char* product;
    int reviewers;
}sum;
*/

int arrayCmp (const void* first, const void* second){
    char* strOne = *(char**)first;
    char* strTwo = *(char**)second;
    return strcmp(strOne, strTwo);
}
/*
void initSum(sum* s) {
    s->evaluation = 0;
    s->product = NULL;
    s->reviewers = 0;
    s->trustNum = 0;
}


sum* getStats(Review** array, int length, int* noProd) {
    sum* sumArray = malloc(sizeof(sum));
    for(int i=0; i < length; i++) {
        if(i > 0 && strcmp(sumArray[*noProd-1].product, array[i].product) == 0 && *noProd > 1) {
            sumArray[*noProd-1].evaluation += array[i].value;
            sumArray[*noProd -1].reviewers +=1;
            sumArray[*noProd-1].trustNum += 1;
        } else {
            sumArray = realloc(sumArray, (*noProd+1)*sizeof(sum));
            initSum(&sumArray[*noProd]);
            sumArray[*noProd].evaluation += array[i].value;
            sumArray[*noProd].reviewers +=1;
            sumArray[*noProd].trustNum += 1;
            (*noProd)++;
        }
    }
    return sumArray;
}
*/

void printStats(Review** array, int no, FILE* f) {
    char** done = malloc(sizeof(char*));
    char* name = NULL;
    int noDone = 0;
    int stop = 0;
    float trust = 0;
    float value = 0;
    int voted = 0;
    for(int i=0; i< no; i++) {
        name = array[i]->product;
        stop = 0;
        value = 0;
        trust = 0;
        for(int i=0; i < noDone; i++) {
            if(strcmp(name, done[i]) == 0) {
                stop = 1;
            }
        }
        if(!stop) {
            done[noDone] = name;
            noDone++;
            done = realloc(done, noDone*sizeof(char*) );
            for(int i=0; i<no;i++) {
                if(strcmp(array[i]->product, name) == 0) {
                    if(array[i]->trustworthy == 'T')
                        trust++;
                    value += array[i]->value;
                    voted++;
                }
            }
            if(f == NULL) {
                printf("%s %0.2f %0.2f\n", name, (float)value/voted, (float)trust/voted);
            } else {
                fprintf(f, "%s %0.2f %0.2f\n", name, (float)value/voted, (float)trust/voted);
            }
        }
    }
    free(done);
}





Review* scanReview(FILE* f) {
    Review* scanned = malloc(sizeof(Review));
    char c = 0;
    char* string = NULL;
    int length = 0;
    fscanf(f,"%d ", &scanned->value);
    fscanf(f,"%c ", &scanned->trustworthy);

    while(c!='\n') {
        c = fgetc(f);
        string = realloc(string, (length+1)*sizeof(char));
        string[length] = tolower(c);
        length++;
    }
    string[length-2] = '\0';
    scanned->product = string;

    return scanned;
}

int main(int argc, char** argv)
{
    if(argc < 3) {
        fprintf(stderr, "Too few arguments!");
        return 1;
    }

    int no;
    char* filename = argv[1];
    char* output = NULL;
    if(argc == 4) {
        output = argv[3];
    }

    sscanf(argv[2], "%d", &no);
    Review **array = malloc(no*sizeof(Review*));


    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        fprintf(stderr, "Problem while opening file!");
        return 2;
    }

    for(int i=0; i<no; i++) {
        array[i] = scanReview(file);
    }

    qsort(array, no, sizeof(char*), &arrayCmp);
    fclose(file);
    file = NULL;

    if(output != NULL) {
        if((file = fopen(output, "w")) == NULL) {
            fprintf(stderr, "Error while opening file");
            return 1;
        }
    }

    printStats(array, no, file);
    for(int i = 0; i<no; i++) {
        free(array[i]);
    }
    free(array);
    if(file != NULL)
            fclose(file);

    return 0;
}

