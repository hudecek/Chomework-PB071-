#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct line {
    int time;
    int bat;
    int giro[2];
    int values[14];
} line;

void printHelp() {
    printf("help:\n");
}

void scanValues(line* l, FILE* f, int* minChannels) {
    fscanf(f, "%d", &l->time);
    fscanf(f, "%d", &l->bat);
    fscanf(f, "%d", &l->giro[0]);
    fscanf(f, "%d", &l->giro[1]);
    for(int i=0; i<14; i++) {
        fscanf(f, "%d", &l->values[i]);
        if(l->values[i] < minChannels[i])
            minChannels[i] = l->values[i];
    }
}

line* scanAll(FILE* f, int* minChannels, int* size) {
    line* l = NULL;
    while(!feof(f)) {
        if(*size % 10 == 0) {
            l = realloc(l, ((*size)+10)*sizeof(line));
        }
        scanValues(&l[*size], f, minChannels);
        (*size)++;
    }
    return l;
}

void removeOffset(line** l, int size, int* minChannels) {
    line* lin = *l;
    for(int j=0; j<size; j++) {
        for(int i = 0; i<14; i++) {
            lin[j].values[i] -= minChannels[i];
        }
    }
}

void printTimed(FILE* file, int from,int to, line* lineArray, int size) {
    for(int i=0; i<size; i++) {
        if(lineArray[i].time < to && lineArray[i].time > from) {
            fprintf(file, "%d ", lineArray[i].time);
            fprintf(file, "%d ", lineArray[i].bat);
            fprintf(file, "%d ", lineArray[i].giro[0]);
            fprintf(file, "%d", lineArray[i].giro[1]);
            for(int j=0; j<14; j++) {
                fprintf(file, " %d", lineArray[i].values[j]);
            }
            fprintf(file, "\n");
        }
    }
}

int main(int argc, char** argv)
{
    int from;
    int to;
    char* filename;
    FILE* file;
    int minChannels[14];
    for(int i=0; i<14; i++) {
        minChannels[i] = INT_MAX;
    }
    line* lineArray;
    int size = 0;

    if(argc == 1) {
        printHelp();
        return 0;
    }

    if(argc != 4) {
        fprintf(stderr, "incorrect number of arguments supplied!");
        return 1;
    }

    sscanf(argv[2], "%d", &from);
    sscanf(argv[3], "%d", &to);

    filename = argv[1];
    file = fopen(filename, "r");

    if(file == NULL) {
        fprintf(stderr, "Problem while opening file");
        return 2;
    }

    lineArray = scanAll(file, minChannels, &size);

    removeOffset(&lineArray, size, minChannels);
    fclose(file);

    char* newName = malloc(strlen(filename)+6);
    sprintf(newName, "%s.noff", filename);
    file = fopen(newName, "w");

    printTimed(file, from, to, lineArray, size);


    fclose(file);
    free(newName);
    free(lineArray);
    return 0;
}

