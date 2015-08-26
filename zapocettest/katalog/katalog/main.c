#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char* name;
    int age;
} Person;

void scanPerson(Person* human, FILE* file) {
    char c = 0;
    int length = 0;
    human->name =NULL;
    fscanf(file, "%d#", &human->age);
    while(c!='\n') {
        c = fgetc(file);
        human->name = realloc(human->name, length+1);
        human->name[length] = c;
        length++;
    }
    human->name[length-1] = '\0';
}

int findPersonsAge(Person* array, int size, char* name) {
    int i = 0;
    int found = 0;
    int age = -1;
    while(!found && i < size) {
        if(strcmp(name, array[i].name) == 0) {
            age = array[i].age;
            found = 1;
        }
        i++;
    }
    return age;
}

int main(int argc, char** argv)
{
    Person* people = NULL;
    FILE* file = NULL;
    int num;

    if(argc != 3) {
        fprintf(stderr,"Incorrect number of arguments passed");
        return 1;
    }

    sscanf(argv[2], "%d", &num);
    file = fopen(argv[1], "r");

    if(file == NULL) {
        fprintf(stderr,"Problem while opening file.");
        return 2;
    }

    for(int i = 0; i< num; i++) {
        people = realloc(people, (i+1)*sizeof(Person));
        scanPerson(&people[i],file);
    }

    printf("Hladana osoba: Daniel Husty, vek: 423\nNajdeny vek: %d\n\n", findPersonsAge(people, num, "Daniel Husty"));
    printf("Hladana osoba: Neexist, vek: ?\nNajdeny vek: %d\n\n", findPersonsAge(people, num, "Neexist"));
    if(num >= 3)
        printf("Tretia osoba: %s\n", people[2].name);


    fclose(file);

    for(int i=0; i<num; i++)
        free(people[i].name);
    free(people);

    return 0;
}



