/*************************************
* Lab 1 Exercise 2
* Name: Kieron Koh
* Student No: A0206059H
* Lab Group: 13
*************************************/

#include "functions.h"

// write the necessary code to initialize the func_list
// array here, if needed

extern void func_list;

void update_functions()
{
    func_list[0] = add_one;
    func_list[1] = add_two;
    func_list[2] = multiply_five;
    func_list[3] = square;
    func_list[4] = cube;

}
