/*************************************
* Lab 1 Exercise 2
* Name:
* Student No:
* Lab Group:
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "function_pointers.h"

// the runner is empty now! Modify it to fulfill the requirements of the
// exercise. You can use ex1.c as a template

// macros
#define SUM_LIST 0
#define INSERT_FROM_HEAD_AT 1
#define INSERT_FROM_TAIL_AT 2
#define DELETE_FROM_HEAD_AT 3
#define DELETE_FROM_TAIL_AT 4
#define RESET_LIST 5
#define MAP 6

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Error: expecting 1 argument, %d found\n", argc - 1);
        exit(1);
    }

    // we read in the file name provided as argument
    char *fname = argv[1];

    // updating the array of function pointers
    // do not remove this call 
    // (you may leave the function empty if you do not need it)
    update_functions();

    // rest of code logic here
}