
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define PI 3.14159265

/**
 * @brief removeEnter - changes the new line character '\n' with ending character - '\0'
 * @param string - string to remove new line character from
 */

void removeEnter(char* string) {
    char* c = strchr(string, '\n');
    if(c) {
        *c = '\0';
    }
}

/**
 * @brief isNum - finds out whether a string contains a number in a correct format
 * @param string - string to investigate
 * @return - 1 if string contains a number, 0 if not
 */

int isNum(char* string) {
    int decimal = 0;
    int containsNumber = 0;
    for(unsigned int i = 0; i<strlen(string); i++) {
        if(string[i]!='-' && string[i]!='.') {
            if(!isdigit(string[i])) {
                return 0;
            } else containsNumber = 1;
        }
        if(string[i]=='-' && i!=0)  // sign can be only a leading character
            return 0;
        if(string[i]=='.') {
            decimal++;
        }
    }
    if(decimal>1 || !containsNumber) // more then one decimal points
        return 0;
    return 1;
}

/**
 * @brief argTest - tests whether there were correct arguments passed
 * @param trans - character representing the type of transformation
 * @param argNum - number of arguments passed
 * @param correctArgs - boolean value - represents the validity of arguments
 * @return - 0 for correct arguments, 2 for incorrect number of arguments, 3 for incorrect argument/-s
 */

int argTest(char trans, int argNum, int correctArgs) {
    int necessaryArgNum;
    if(trans == 'r') {
        necessaryArgNum = 3;
    } else {   // other transformations than rotation require 4 arguments
        necessaryArgNum = 4;
    }
    if (argNum != necessaryArgNum) {
        return 2;
    }
    if(!correctArgs) {
        return 3;
    }
    return 0;
}

/**
 * @brief printError - prints an error message according to an error number
 * @param errNum - number of an error that occured
 */

void printError(int errNum) {
    switch(errNum) {
    case 1:
        fprintf(stderr,"neznama transformace!");
        break;
    case 2:
        fprintf(stderr,"chybny format!");
        break;
    case 3:
        fprintf(stderr,"chybny argument!");
        break;
    }
}

/**
 * @brief multiplyMatrix - function conducts a multiplication of a 3x3 type matrix and 1x3 type vector
 *      (== linear transformation)
 * @param matrix - matrix of a transformation
 * @param vector - vector to transform
 * @param result - pointer to an array for storing the result of multiplication
 */

void multiplyMatrix(double matrix[3][3], double vector[3], double* result) {

    for(int i=0; i<3; i++) {
        result[i] = 0;
        for(int j=0; j<3; j++) {
            result[i] += matrix[i][j]*vector[j];
        }
    }
}

/**
 * @brief rotate - conducts a rotation of a point (x,y) around (0,0) by a given angle
 * @param x - x-coordinate of a point
 * @param y - y-coordinate of a point
 * @param angle - angle of rotation in radians
 * @param result - pointer to an array for storing the result
 */

void rotate(double x, double y, double angle, double* result) {
    double matrix[3][3] = {{cos(angle),-sin(angle),0},
                          {sin(angle),cos(angle),0},
                          {0,0,1}};
    double vector[3] = {x,y,1};
    multiplyMatrix(matrix, vector, result);
}

/**
 * @brief translate - moves a given point by a given distance on a x and y axis
 * @param x - x-coordinate of a point
 * @param y - y-coordinate of a point
 * @param translateX - distance to move on x asis
 * @param translateY - distance to move on y axis
 * @param result - pointer to an array for storing the result
 */

void translate(double x, double y, double translateX, double translateY, double* result) {
    double matrix[3][3] = {{1, 0, translateX},
                           {0, 1, translateY},
                           {0, 0, 1}};
    double vector[3] = {x, y, 1};
    multiplyMatrix(matrix,vector, result);
}

/**
 * @brief scale - changes the coordinates of a given point according to a given scale
 * @param x - x-coordinate of a point
 * @param y - y-coordinate of a point
 * @param scaleX - displays the ratio of x-coordinate change (1.0 = identity)
 * @param scaleY - displays the ratio of y-coordinate change (1.0 = identity)
 * @param result - pointer to an array for storing result
 */

void scale(double x, double y, double scaleX, double scaleY, double* result) {
    double matrix[3][3] = {{scaleX, 0, 0},
                          {0, scaleY, 0},
                          {0, 0, 1}};
    double vector[3] = {x, y, 1};
    multiplyMatrix(matrix, vector, result);
}

int main(void)
{
    char challenge[50];
    fgets(challenge, 50, stdin);

    removeEnter(challenge);

    if(strlen(challenge)==0) {
        printError(1);
        return 1;
    }
    double arguements[5];

    /*
     * first character represents a choice (r/s/t)
     * -> if length of first part of the input > 1 char == unknown transformati$
     * we're taking into account just first character -> change to f. ex. 'x')
    */

    char transformation;

    char *part = strtok(challenge, " ");
    if(strlen(part)!=1) {
        transformation = 'x';
    } else {
        transformation = part[0];
    }

    int correctArg = 1;
    int argCount = 0;

    part = strtok(NULL, " ");

    while(part !=NULL) {

       if(isNum(part)) {
           arguements[argCount] = atof(part);
       } else
           correctArg = 0; // argument is incorrect -> invalid input
        argCount++;
        part = strtok(NULL, " ");
    }

    int error = argTest(transformation, argCount, correctArg);
    double resultArray[3];

    switch(transformation) {
    case 'r':
        if(error) {
            printError(error);
            return error;
        }

        rotate(arguements[0], arguements[1], (2*PI*arguements[2])/360, resultArray);
        printf("[%.2f, %.2f]\n", resultArray[0], resultArray[1]);

        break;
    case 't':
        if(error) {
            printError(error);
            return error;
        }

        translate(arguements[0], arguements[1], arguements[2], arguements[3], resultArray);
        printf("[%.2f, %.2f]\n", resultArray[0], resultArray[1]);
        break;

    case 's':
        if(error) {
            printError(error);
            return error;
        }

        scale(arguements[0], arguements[1], arguements[2], arguements[3], resultArray);
        printf("[%.2f, %.2f]\n", resultArray[0], resultArray[1]);
        break;

    default:
        printError(1);
        return 1;
    }

    return 0;
}

