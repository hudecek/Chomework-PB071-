#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#include "hash_helper.h"
#include <string.h>

struct information {
    unsigned char doXor;
    unsigned short doCrc16;
    unsigned int doCrc32;
    unsigned int doMd5;
    char hex;
};

void initializeInformation(struct information* info) {
    info->doCrc16 = 0;
    info->doCrc32 = 0;
    info->doMd5 = 0;
    info->doXor = 0;
    info->hex = 0;
}


int getArgs(int argc, char* argv[], char** filename, struct information* data) {
    if(argc < 2) {
        fprintf(stderr,"Insuficient number of arguments supplied!");
        return 1;
    }
    int i=1;
    while (i < argc) {
        if(*filename == NULL && strcmp(argv[i], "-f") == 0) {
            if(argv[i+1][0] == '-') {
                fprintf(stderr, "No file specified after -f!");
                return 2;
            } else {
                *filename = argv[++i];
            }
        } else {
            if(strcmp(argv[i], "-hex") == 0)
                data->hex = 1;
            else if(strcmp(argv[i], "-xor") == 0)
                data->doXor = 1;
            else if(strcmp(argv[i], "-c16") == 0)
                data->doCrc16 = 1;
            else if(strcmp(argv[i], "-c32") == 0)
                data->doCrc32 = 1;
            else if(strcmp(argv[i], "-md5") == 0)
                data->doMd5 = 1;
            else {
                fprintf(stderr, "Incorrect argument supplied!");
                return 2;
            }
        }
        i++;
    }
    return 0;
}

void printChosen(char* s, unsigned size, struct information info) {
    printf("Length: %u bytes\n", size);
    if(info.hex) {
        if(info.doXor)
            printf("XOR: 0x%02x\n", xor_hash(s, size));
        if(info.doCrc16)
            printf("CRC-16: 0x%0x\n", crc_16(s, size));
        if(info.doCrc32)
            printf("CRC-32: 0x%0x\n", crc_32(s, size));
    } else {
        if(info.doXor)
            printf("XOR: %u\n", xor_hash(s, size));
        if(info.doCrc16)
            printf("CRC-16: %u\n", crc_16(s, size));
        if(info.doCrc32)
            printf("CRC-32: %u\n", crc_32(s, size));
    }
    if(info.doMd5) {
        printf("MD5: ");
        printMD5(s, size);
        printf("\n");
    }

}

int main(int argc, char* argv[])
{
    char* fname = NULL;
    struct information info;
    initializeInformation(&info);
    int error = getArgs(argc, argv, &fname, &info);
    unsigned fileSize = 0;
    char* s;

    if(error) {
        return error;
    }
    FILE* file = NULL;
    if(fname == NULL)
        file = stdin;
    else file = fopen(fname,"r");

    if(!file) {
        fprintf(stderr,"The file %s doesn't exist!", argv[argc-1]);
        return 3;
    }

    if(file == stdin) {
        s = malloc(50);
        s[0] = fgetc(stdin);
        while(s[fileSize]!=EOF) {
            fileSize++;
            if(fileSize % 50 == 0) {
                s = realloc(s, fileSize +50);
                if(s == NULL)
                        return 4;
            }
            s[fileSize] = fgetc(stdin);
        }
    } else {
        rewind(file);
        fseek(file, 0L, SEEK_END);
        fileSize = ftell(file);
        rewind(file);
        s = malloc(fileSize);
        fread(s, fileSize, 1, file);
    }

    printChosen(s, fileSize, info);
    free(s);
    fclose(file);
    return 0;
}

