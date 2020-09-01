/*************************************
* Lab 1 Exercise 2
* Name: Kieron Koh
* Student No: A0206059H
* Lab Group: 13
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

void run_instruction(list*lst, int instr, int index, int data);
extern int (*func_list[5])(int);
//void print_list(list *lst); //for testing!

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
    FILE *fpointer;
    fpointer = fopen(fname, "r");
   
    if (fpointer == NULL) {
        printf("ERROR!");
    }

       
    list *lst = (list *)malloc(sizeof(list));
    lst->head = NULL;
    lst->tail = NULL;
   
   char arr[100];
   
   while (fgets(arr, sizeof(arr), fpointer) != NULL)
   {   
       int first;
       int second;
       int third;

       sscanf(arr, "%d" "%d" "%d", &first, &second, &third);

       if (first == 0 || first == 5) {
            second = 0;
            third = 0;
       }

       else if (first == 6) {
            third = 0;
       }

       else {}

   //    printf("%d, %d, %d\n", first, second, third); //For testing!
   //    print_list(lst); //For testing!
       run_instruction(lst, first, second, third);
       
   }
    
    reset_list(lst);   
    free(lst);
    fclose(fpointer);
}


void run_instruction(list *lst, int instr, int index, int data)
{
    switch (instr)
    {
    case SUM_LIST:
        sum_list(lst);
        break;
    case INSERT_FROM_HEAD_AT:
        insert_node_from_head_at(lst, index, data);
        break;
    case INSERT_FROM_TAIL_AT:
        insert_node_from_tail_at(lst, index, data);
        break;
    case DELETE_FROM_HEAD_AT:
        delete_node_from_head_at(lst, index);
        break;
     case DELETE_FROM_TAIL_AT:
        delete_node_from_tail_at(lst, index);
        break;   
    case RESET_LIST:
        reset_list(lst);
        break;
    case MAP:
        map(lst, func_list[index]);
 
    }
}



//void print_list(list *lst) //For testing!
//{
//    printf("Forward: [ ");
//    node *curr = lst->head;
//    while (curr != NULL)
//    {
//        printf("%d ", curr->data);
//        curr = curr->next;
//    }
//
//    printf("], Backwards: [ ");
//    curr = lst->tail;
//    while (curr != NULL)
//    {
//        printf("%d ", curr->data);
//        curr = curr->prev;
//    }
//    printf("]\n");
//}
//




  

