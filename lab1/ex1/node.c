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
    p = (struct NODE*) malloc(sizeof(struct NODE)); // allocating memory for the linked list
    int i;
    
    p->data = data; // set the data of inserted node as data

    current = lst->head; //set current to the first node (index 0)
    
    if (current == NULL) { //if list is empty!
        lst->head = p;
        lst->tail = p;
    }

    else {

        for (i=0; i< index - 1; i++) {
            current = current->next;
        }
            
        if (index == 0) { //inserting at the head of the linked list
            p->next = current;
            p->prev = NULL;
            current->prev = p;
            lst->head = p;
        }
        
        else if (current->next == NULL) { //inserting at the end of the linekd list
            current->next = p;
            p->prev = current;
            p->next = NULL;
            lst->tail = p;
        }

        else {
            p->next = current->next;
            p->next->prev = p;
            current->next = p;
            p->prev = current;
        }
    }
}

// inserts a new node with data value at index (counting from the back
// starting at 0)
void insert_node_from_tail_at(list *lst, int index, int data)
{
    struct NODE *p; //declare a pointer p
    struct NODE *current; //declare a current pointer to traverse the linked list
    p = (struct NODE*) malloc(sizeof(struct NODE)); // allocating memory for the linked list
    int i;
    
    p->data = data; // set the data of inserted node as data

    current = lst->tail; //set current to the first node (index 0)
    
    if (current == NULL) { //if list is empty!
        lst->head = p;
        lst->tail = p;
    }

    else {

        for (i=0; i< index - 1; i++) {
            current = current->prev;
        }
            
        if (index == 0) { //inserting at the tail of the linked list
            p->prev = current;
            p->next = NULL;
            current->next = p;
            lst->tail = p;
        }
        
        else if (current->prev == NULL) { //inserting at the start of the linekd list
            current->prev = p;
            p->next = current;
            p->prev = NULL;
            lst->head = p;
        }

        else {
            p->prev = current->prev;
            p->prev->next = p;
            current->prev = p;
            p->next = current;
        }
    }
}

// deletes node at index counting from the front (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_head_at(list *lst, int index)
{
    struct NODE *current; //declare a current pointer to traverse the linked list
    int i;
    
    current = lst->head; //set current to the first node (index 0)
    
    if (current == NULL || lst->tail == lst->head) {
        lst->head = NULL;
        lst->tail = NULL;
    }

    else {

        for (i=0; i< index; i++) {
            current = current->next;
        }
            
        if (index == 0) { //deleting at the tail of the linked list
            lst->head = current->next;
            current->next->prev = NULL;
        }
        
        else if (current->next == NULL) { //deleting at the start of the linked list
            current->prev->next = NULL;
            lst->tail = current->prev;
        }

        else {
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }
    }

    if (current) { //free pointer
        free(current);
    }
}

// deletes node at index counting from the back (starting from 0)
// note: index is guaranteed to be valid
void delete_node_from_tail_at(list *lst, int index)
{
  struct NODE *current; //declare a current pointer to traverse the linked list
    int i;
    
    current = lst->tail; //set current to the first node (index 0)
    
    if (current == NULL || lst->tail == lst->head) {
        lst->head = NULL;
        lst->tail = NULL;
    }

    else {

        for (i=0; i< index; i++) {
            current = current->prev;
        }
            
        if (index == 0) { // deleting at the tail of the linked list
            lst->tail = current->prev;
            current->prev->next = NULL;
        }
        
        else if (current->prev == NULL) { //deleting at the start of the linked list
            current->next->prev = NULL;
            lst->head = current->next;
        }

        else {
            current->next->prev = current->prev;
            current->prev->next = current->next;
        }
    }

    if (current) { //free pointer
        free(current);
    }

}

// resets list to an empty state (no nodes) and frees any allocated memory in
// the process
void reset_list(list *lst)
{
    struct NODE *current;
    struct NODE *temp;

    current = lst->head;
    
    lst->head = NULL;
    lst->tail = NULL;

    while (current != NULL) {
        temp = current;
        current = current->next;
    
        if (temp) {
            free(temp);
        }
    }
}
