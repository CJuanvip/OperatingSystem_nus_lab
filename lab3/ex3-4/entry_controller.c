/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "entry_controller.h"
#include <semaphore.h>

void entry_controller_init( entry_controller_t *entry_controller, int loading_bays ) {
    entry_controller->loading_bays = loading_bays;
    entry_controller->into_bay = malloc(sizeof(sem_t));
    entry_controller->lock = malloc(sizeof(sem_t));
    
    sem_init(entry_controller->into_bay, 0, entry_controller->loading_bays);
    sem_init(entry_controller->lock, 0, 1);
}

void entry_controller_wait( entry_controller_t *entry_controller ) {
}

void entry_controller_post( entry_controller_t *entry_controller ) {
}

void entry_controller_destroy( entry_controller_t *entry_controller ) {
}

