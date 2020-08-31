/*************************************
* Lab 1 Exercise 2
* Name:
* Student No:
* Lab Group:
*************************************/

#include "functions.h"

// write the necessary code to initialize the func_list
// array here, if needed

void update_functions() 
{
    int (*array_of_functions[5])(int);

    array_of_functions[0] = add_one;
    array_of_functions[1] = add_two;
    array_of_functions[2] = multiply_five;
    array_of_functions[3] = square;
    array_of_functions[4] = cube;

}
