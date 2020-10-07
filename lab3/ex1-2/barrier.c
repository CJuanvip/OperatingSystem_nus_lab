/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */
#include "barrier.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) {
    barrier->count = count;
    barrier->val = 0;
    barrier->sem = malloc(sizeof(sem_t));
    barrier->lock = malloc(sizeof(sem_t));

    sem_init(barrier->sem, 0, 1);
    sem_init(barrier->lock, 0, 1);
}

void barrier_wait ( barrier_t *barrier ) {
    sem_wait(barrier->sem);
    if (barrier->val == 0) {    
        sem_wait(barrier->lock);
    }
        
    barrier->val += 1;
    
    if (barrier->val == barrier->count) {
        barrier->val = 0;
        sem_post(barrier->lock);
    }
    sem_post(barrier->sem);
    
    sem_wait(barrier->lock);
    sem_post(barrier->lock);
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) {
    sem_destroy(barrier->sem);
    sem_destroy(barrier->lock);

    free(barrier->sem);
    free(barrier->lock);
}
