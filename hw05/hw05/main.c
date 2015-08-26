/*
 * Program acts as a UNIX find shell app
 * error codes:
 * 1 - missing mandatory argument
 * 2 - unrecognized argument
 * 3 - unable to open start directory
 * 4 - problem with getting stats
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h> //info about the file (owner)
#include <errno.h>


typedef struct options {
    char* substring;
    char* ext;
    char* sort;
    struct passwd* user;
    bool hidden;
} options;

/**
 * @brief initializeOptions - initializes values of options type
 * @param o
 */
void initializeOptions(options* o) {
    if(o!=NULL) {
        o->ext = NULL;
        o->substring = NULL;
        o->sort = NULL;
        o->user = NULL;
        o->hidden = false;
    }
}

/**
 * @brief printHelp - prints message explaining the use of program
 */
void printHelp() {
    printf("USAGE: START_DIR [OPTIONS]\nOPTIONS:\n"
           "\t-h 		 Show help\n"
           "\t-n NAME 	 Finding by substring in name\n"
           "\t-e EXTENTION 	 Finding by extension\n"
           "\t-s SORT 	 Sorting by {Extention, Size, Name}\n"
           "\t-u USER 	 Find files by user name.\n"
           "\t-a 		 Go throught hidden dirs.\n"
           "\t-v 		 Verbose [OPTIONAL]\n\n"
           "SORT:\n"
           "\t\"s\" 		 Sort by size.\n"
           "\t\"e\" 		 Sort by extention.\n"
           "\t\"es\"		 Sort by extention and if conflict then by size.\n"
           "\t\"se\"		 Sort by size and if conflict then by extention.\n"
           "\t\"f\" 		 Sort by full/relative file path.\n");
}

/**
 * @brief dealWithArguments - fills the options variable with values supplied to the program
 * @param argc
 * @param argv
 * @param op
 * @return - 0 if correct, error value if there was a problem
 */
int dealWithArguments(int argc, char** argv, options* opt) {
    char c = 0;
    if(opt!=NULL) {
        while((c = getopt(argc, argv, ":ahn:e:s:u:")) != -1) {
            switch(c) {
            case 'a':
                opt->hidden = true;
                break;
            case 'h':
                printHelp();
                return -1;
                break;
            case 'n':
                opt->substring = optarg;
                break;
            case 'e':
                opt->ext = optarg;
                break;
            case 's':
                opt->sort = optarg;
                break;
            case 'u':
                if((opt->user = getpwnam(optarg)) == NULL)
                    opt->user = NULL;
                break;
            case ':':
                fprintf(stderr, "Error: No argument supplied to -%c!\n", optopt);
                return 1;
                break;
            case '?':
                fprintf(stderr, "Error: Unrecognized argument: -%c\n", optopt);
                return 2;
                break;
            }
        }
    }
    return 0;
}

/**
 * @brief isHidden - decides whether the file should be hidden
 * @param filename
 * @return 1 if hidden, otherwise 0
 */
int isHidden(char* filename) {
    if(filename!=NULL) {
        if(filename[0] == '.')
            return 1;
    }
    return 0;
}

/**
 * @brief getFileFromPath - gets the filename.extenstion part from path
 * @param path - path to the file
 * @return - full filename (including extension)
 */
const char* getFileFromPath(const char* path) {
    if(path!=NULL) {
        char* lastSlash = strrchr(path, '/');
        if(lastSlash == NULL)
            return path;
        return lastSlash + 1;
    }
    return NULL;
}

/**
 * @brief getExt - function gets the pointer to the file extension
 * @param filename
 * @return - pointer to the first character of file extension
 */
char *getExt(const char *filename) {
    if(filename!=NULL) {
        const char* fname = getFileFromPath(filename);
        char* lastDot = strrchr(fname, '.');
        if(lastDot == NULL || lastDot == fname)
            return "";
        return lastDot + 1;
    }
    return NULL;
}

/**
 * @brief getFileName - gets only filename from the path
 * @param path - path to the file
 * @return - filename (without extension)
 */
char* getFileName(const char* path) {
    if(path!=NULL) {
        const char* file = getFileFromPath(path);
        const char* ext = getExt(file);
        char* filename;
        if(strcmp(ext, "") != 0) {
            filename = malloc(strlen(file) - strlen(ext));
            strncpy(filename, file, strlen(file)-strlen(ext)-1);
            filename[strlen(file)-strlen(ext)-1] = '\0';
        } else {
            if(file[strlen(file)-1] != '.') {
                filename = malloc(strlen(file) + 1);
                strncpy(filename, file, strlen(file));
                filename[strlen(file)] = '\0';
            } else {
                filename = malloc(strlen(file));
                strncpy(filename, file, strlen(file)-1);
                filename[strlen(file)-1] = '\0';
            }
        }
        return filename;
    }
    return NULL;
}

/**
 * @brief fitsRequirements - finds out whether the entry fits requirements
 * @param walker
 * @param opt
 * @return 1 if fits, 0 if not
 */
int fitsRequirements(struct dirent* walker, options* opt, struct stat status) {
    if(walker!=NULL && opt!=NULL) {
        struct passwd* owner = NULL;
        char* extension = NULL;
        char* fname = NULL;
        if(opt->user != NULL) {
            owner = getpwuid(status.st_uid);
            if(owner->pw_uid != opt->user->pw_uid)
                return 0;
        }
        if(opt->ext != NULL) {
            extension = getExt(walker->d_name);
            if(extension == NULL)
                return 0;
            else {
                if(strcmp(extension, opt->ext)!=0)
                    return 0;
            }
        }
        if(opt->substring != NULL) {
            fname = getFileName(walker->d_name);
            if(strstr(fname, opt->substring) == NULL) {
                free(fname);
                return 0;
            }
            free(fname);
        }
        if(!opt->hidden) {
            if(isHidden(walker->d_name))
                return 0;
        }
        return 1;
    }
    return 0;
}

/**
 * @brief extCmp - compares extensions of two files - case-insensitive
 * @param first - path to the first file
 * @param second - path to the second file
 * @return - + if ext1 > ext2, 0 if equal, - if ext1 < ext2
 */
int extCmp(const void* first, const void* second) {
    char* firstExt = getExt(*(char**)first);
    char* secondExt = getExt(*(char**)second);
    return strcasecmp(firstExt, secondExt);
}

/**
 * @brief pathCmp - comparison by path - case-insensitive
 * @param first - path to the first file
 * @param second - path to the second file
 * @return - + if path1 > path2, 0 if equal, - if path1 < path2
 */
int pathCmp(const void* first, const void* second) {
    return strcmp(*(char**)first, *(char**)second);
}

/**
 * @brief nameCmp - comparison by name - case-insensitive
 * @param first - path to the first file
 * @param second - path to the second file
 * @return - + if name1 > name2, pathCmp if equal, - if name1 < name2
 */
int nameCmp(const void* first,const void* second) {
    char* firstName = getFileName(*(char**)first);
    char* secondName = getFileName(*(char**)second);
    int result = strcasecmp(firstName, secondName);
    if(result == 0) {
        result = pathCmp(first, second);
    }
    free(firstName);
    free(secondName);
    return result;
}

/**
 * @brief sizeCmp - comparison by size
 * @param first
 * @param second
 * @return - + if size1 < size2, 0 if equal, - if size1 > size2 --biggerfirst
 */
int sizeCmp(const void* first, const void* second) {
    struct stat statusOne;
    struct stat statusTwo;
    stat(*(char**)first, &statusOne);
    stat(*(char**)second, &statusTwo);
    if(statusOne.st_size < statusTwo.st_size) //different < > signs
        return 1;
    if(statusOne.st_size > statusTwo.st_size)
        return -1;
    return 0;
}

/**
 * @brief extNameCmp - comparison by extension and name if equal
 * @param first
 * @param second
 * @return - + if ext1 > ext2, nameCmp if equal ext, - if ext1 < ext2
 */
int extNameCmp(const void* first,const void* second) {
    int result = extCmp(first, second);
    if(result == 0)
        result = nameCmp(first, second);
    return result;
}

/**
 * @brief sizeNameCmp - comparison by size and name if equal
 * @param first
 * @param second
 * @return
 */
int sizeNameCmp(const void* first, const void* second) {
    int result = sizeCmp(first, second);
    if(result == 0)
        result = nameCmp(first, second);
    return result;
}


/**
 * @brief extSizeCmp - comparison by ext and size if equal
 * @param first
 * @param second
 * @return
 */
int extSizeCmp(const void* first,const void* second) {
    int result = extCmp(first, second);
    if(result == 0)
        result = sizeNameCmp(first, second);
    return result;
}

/**
 * @brief sizeExtCmp - comparison by size and ext if equal
 * @param first
 * @param second
 * @return
 */
int sizeExtCmp(const void* first,const void* second) {
    int result = sizeCmp(first, second);
    if(result == 0)
        result = extNameCmp(first, second);
    return result;
}

/**
 * @brief sortArray - sorts the array according given parameters
 * @param array
 * @param arraySize
 * @param opt - parameters
 */
void sortArray(char*** array, int* arraySize, options* opt) {
    if(array!=NULL && arraySize!=NULL && opt!=NULL) {
        char** arr = *array;
        int (*operation)(const void*, const void*) = NULL;
        if(opt->sort != NULL) {
            if(strcmp(opt->sort, "e") == 0) {
                operation = &extNameCmp;
            } else if(strcmp(opt->sort, "s") == 0) {
                operation = &sizeNameCmp;
            } else if(strcmp(opt->sort, "f") == 0) {
                operation = &pathCmp;
            } else if(strcmp(opt->sort, "es") == 0) {
                operation = &extSizeCmp;
            } else  if(strcmp(opt->sort, "se") == 0) {
                operation = &sizeExtCmp;
            }
        } else {
            operation = &nameCmp;
        }

        qsort(arr, *arraySize, sizeof(char*), operation);
    }

}

/**
 * @brief addToArray - adds a string to array
 * @param filename - pointer to a string
 * @param array - array for storing results
 * @param arraySize
 */
void addToArray(char* filename, char*** array, int* arraySize) {
    if(array!=NULL && arraySize!=NULL && filename!=NULL) {
        char** arr = *array;
        if (*arraySize % 10 == 0) {
            arr = (char**) realloc(arr, sizeof(char*)*(*arraySize + 10));
        }
        arr[*arraySize] = filename;
        (*arraySize)++;
        *array = arr;
    }
}


/**
 * @brief printFilesRec - recursively searches for a file with specified parameters
 * @param opened
 * @param opt
 */
int getFilesRec(DIR* opened, options* opt, char** dirpath, char*** results, int* resNum) {
    if(opened!=NULL && opt!=NULL && dirpath!=NULL && results!=NULL) {
        struct dirent* walker = NULL;
        char* filepath = NULL;
        DIR* nextDir = NULL;
        struct stat status;

        while((walker = readdir(opened))!=NULL) {
            filepath = malloc(strlen(*dirpath) + strlen(walker->d_name) + 2);

            if(filepath!=NULL) {
                strcpy(filepath, *dirpath);
                strcat(filepath, "/");
                strcat(filepath, walker->d_name);
            }

            if(stat(filepath, &status) < 0) {

                fprintf(stderr, "Problem while getting stats: %s\nError: %s\n",filepath,strerror(errno));
                return 4;
            }

            if(!S_ISDIR(status.st_mode)) {
                if(fitsRequirements(walker, opt, status)) {
                    addToArray(filepath, results, resNum);
                } else {
                    free(filepath);
                }

            } else {
                if((strcmp(walker->d_name, ".")!=0) && (strcmp(walker->d_name, "..")!=0)) {
                    if(opt->hidden || isHidden(walker->d_name) != 1) {
                        if((nextDir = opendir(filepath))!=NULL) {
                            getFilesRec(nextDir, opt, &filepath, results, resNum);
                            closedir(nextDir);
                        } else {
                            fprintf(stderr, "Problem while entering directory: %s\nError: %s\n", filepath, strerror(errno));
                        }
                    }
                }
                free(filepath);
            }
        }
    }
    return 0;
}

/**
 * @brief freeAll - frees all members of dynamically alocated array and the array itself
 * @param array
 * @param size
 */
void freeAll(char** array, int* size) {
    if(array!=NULL && size!=NULL) {
        for(int i=0; i<*size; i++) {
            free(array[i]);
        }
        free(array);
    }
}

int main(int argc, char** argv) {
    options opt;
    initializeOptions(&opt);
    char* path;
    DIR* startDir;
    char** resultsArray = NULL;
    int size = 0;

    if(argv[1][0] != '-')
        path = argv[1];
    else path = argv[0];
    if(strlen(path) > 1 &&  path[strlen(path)-1] == '/') {
        path[strlen(path)-1] = '\0';
    }
    if(strlen(path) > 1 &&  path[strlen(path)-1] == '/') {
        path[strlen(path)-1] = '\0';
    }

    int error = dealWithArguments(argc, argv, &opt);
    if(error > 0) {
        return error;
    } else if(error == -1)
        return 0;
    startDir = opendir(path);

    if(startDir!=NULL) {
        error = getFilesRec(startDir, &opt, &path, &resultsArray, &size);
        sortArray(&resultsArray, &size, &opt);
        for(int i=0;i<size;i++) {
            printf("%s\n", resultsArray[i]);
        }
        freeAll(resultsArray, &size);
        closedir(startDir);
        if(error!=0) {
            fprintf(stderr, "Unexpected error happened");
            return error;
        }
    } else {
        fprintf(stderr, "Error while opening directory");
        return 3;
    }

    return 0;
}

