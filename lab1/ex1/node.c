/*************************************
* Lab 1 Exercise 1
* Name: Kieron Koh
* Student No: A0206059H
* Lab Group: 13
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "node.h"

// add in your implementation below to the respective functions
// feel free to add any headers you deem fit (although you do not need to)

// inserts a new node with data value at index (counting from the front
// starting at 0)
void insert_node_from_head_at(list *lst, int index, int data)
{  
    struct NODE *p; //declare a pointer p
    struct NODE *current; //declare a current pointer to traverse the linked list
    p = (struct*) malloc(sizeOf(struct)); // allocating memory for the linked list
    int i;
    
    p->data = data; // set the data of inserted node as data

    current = lst->head; //set current to the first node (index 0)
    
    for (i=0; i< index; i++) {
        current = current->next;
    }
        
    if (current->prev == NULL) { //inserting at the head of the linked list
        p->next = current;
        p->prev = NULL;
        lst->head = p;
    }
    
    else if (current->next == NULL) { //inserting at the end of the linekd list
        current->next = p;
        p->prev = current;
        p->next = NULL;
        lst->tail = p;
    }

    else {
        p->next = current;
        p->prev = current->prev;
        current->prev->next = p;
    }

}

// inserts a new node with data value at index (counting from the back
// starting at 0)
void insert_node_from_tail_at(list *lst, int index, int data)
{
}

// deletes node at index counting from the front (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_head_at(list *lst, int index)
{
}

// deletes node at index counting from the back (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_tail_at(list *lst, int index)
{
}

// resets list to an empty state (no nodes) and frees any allocated memory in
// the process
void reset_list(list *lst)
{
}
