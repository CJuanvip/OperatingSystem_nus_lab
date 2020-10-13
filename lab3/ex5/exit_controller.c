/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains declarations. You should only modify the fifo_sem_t struct,
 * as the method signatures will be needed to compile with the runner.
 */
#include "exit_controller.h"
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

void exit_controller_init(exit_controller_t *exit_controller, int no_of_priorities) {
    int i, j;
    printf("%d",no_of_priorities); 
    exit_controller->number = no_of_priorities;
    exit_controller->lock = malloc(sizeof(sem_t));
    exit_controller->exit = malloc(sizeof(sem_t));

    sem_init(exit_controller->lock, 0, 1);
    sem_init(exit_controller->exit, 0, 1);

    for (i = 0; i < exit_controller->number; i++) {
        exit_controller->counter[i] = 0;
    }

    for (j = 0; j < exit_controller->number; j++) {
        exit_controller->priorities[j] = malloc(sizeof(sem_t));
        sem_init(exit_controller->priorities[j], 0, 0);
    }

}

void exit_controller_wait(exit_controller_t *exit_controller, int priority) {
    int i, sval = 0;
    sem_wait(exit_controller->lock);
        exit_controller->counter[priority] += 1;
        for (i = 0; i < exit_controller->number; i++) {
            sval += exit_controller->counter[i]; 
        }
        if (sval == 1) { // If first/only train to be exited
            sem_post(exit_controller->priorities[priority]);
        }
    sem_post(exit_controller->lock);
     
    sem_wait(exit_controller->priorities[priority]);
    sem_wait(exit_controller->exit);
}

void exit_controller_post(exit_controller_t *exit_controller, int priority) {
    int i, j, result = -1;
    sem_wait(exit_controller->lock);
        exit_controller->counter[priority] -= 1;
        for (i = 0; i < exit_controller->number; i++) {
            if (exit_controller->counter[i] != 0) {
                result = i;
                break;
            }
        }
    sem_post(exit_controller->lock);
    if (result != -1) {
        sem_post(exit_controller->priorities[result]);
    }
    else { // If there is no more trains waiting to exit
        for (j = 0; j < exit_controller->number; j++) {
            sem_post(exit_controller->priorities[j]);
        }
    }
    sem_post(exit_controller->exit);

}

void exit_controller_destroy(exit_controller_t *exit_controller){
    int i;
    free(exit_controller->lock);
    free(exit_controller->exit);

    sem_destroy(exit_controller->lock);
    sem_destroy(exit_controller->exit);

    for (i = 0; i < exit_controller->number; i++) {
        free(exit_controller->priorities[i]);
        sem_destroy(exit_controller->priorities[i]);
    }   
}
