/*
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "entry_controller.h"
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

void entry_controller_init( entry_controller_t *entry_controller, int loading_bays ) {
    int i;
    entry_controller->loading_bays = loading_bays;
    entry_controller->into_bay = malloc(sizeof(sem_t));
    entry_controller->lock = malloc(sizeof(sem_t));
    entry_controller->front = 0;
    entry_controller->back = 0;
   
    sem_init(entry_controller->into_bay, 0, entry_controller->loading_bays);
    sem_init(entry_controller->lock, 0, 1);
   
    for (i = 0; i < ENTRY_CONTROLLER_MAX_USES; i++) { 
        entry_controller->trains[i] = malloc(sizeof(sem_t));
        sem_init(entry_controller->trains[i], 0, 0);
    }
}

void entry_controller_wait( entry_controller_t *entry_controller ) {
    int sval, count, checker;
    
    sem_wait(entry_controller->lock);
        sem_getvalue(entry_controller->into_bay, &sval); //If loading bay is full
        checker = sval;
        if (sval == 0) {
            count = entry_controller->back;
            if (entry_controller->back + 1 == 5000) {
                entry_controller->back = 0;
            }
            else {
                entry_controller->back += 1;
            }
        }
    sem_post(entry_controller->lock);
    
    if (checker == 0) {    
        sem_wait(entry_controller->trains[count]);
    }
    sem_wait(entry_controller->into_bay);
}

void entry_controller_post( entry_controller_t *entry_controller ) {
    sem_post(entry_controller->trains[entry_controller->front]);
    sem_wait(entry_controller->lock);    
    if (entry_controller->front + 1 == 5000) {
        entry_controller->front = 0;
    }
    else {
        entry_controller->front += 1;
    }
    sem_post(entry_controller->lock);
    sem_post(entry_controller->into_bay);

}

void entry_controller_destroy( entry_controller_t *entry_controller ) {
    int i;
    free(entry_controller->into_bay);
    free(entry_controller->lock);
   
    sem_destroy(entry_controller->into_bay);
    sem_destroy(entry_controller->lock);
    
    for (i = 0; i < ENTRY_CONTROLLER_MAX_USES; i++) {
        free(entry_controller->trains[i]);
        sem_destroy(entry_controller->trains[i]);
    }
}


