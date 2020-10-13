/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "fizzbuzz_workers.h"
#include "barrier.h" // you may use barriers if you think it can help your
#include <stdlib.h> 
#include <stdio.h>
#include <semaphore.h>
// implementation
// declare variables to be used here
barrier_t *barrier;
barrier_t *barrier2;

void fizzbuzz_init ( int n ) {
    barrier = malloc(sizeof(barrier_t)); 
    barrier_init(barrier, 4);
    barrier2 = malloc(sizeof(barrier_t)); 
    barrier_init(barrier2, 4);
}

void num_thread( int n, void (*print_num)(int) ) {
    int i;
    for (i = 1; i < n+1; i++) {
        barrier_wait(barrier);
        if (i % 3 != 0 && i % 5 != 0) {
            (*print_num)(i);
        }
        barrier_wait(barrier2);
    }
}

void fizz_thread( int n, void (*print_fizz)(void) ) {
   int i;
    for (i = 1; i < n+1; i++) {
        barrier_wait(barrier);
        if (i % 3 == 0 && i % 5 != 0) {
            (*print_fizz)();
        }
        barrier_wait(barrier2);
    }
}

void buzz_thread( int n, void (*print_buzz)(void) ) {
    int i;
    for (i = 1; i < n+1; i++) {
        barrier_wait(barrier);
        if (i % 3 != 0 && i % 5 == 0) {
            (*print_buzz)();
        }
        barrier_wait(barrier2);
    } 
}

void fizzbuzz_thread( int n, void (*print_fizzbuzz)(void) ) {
    int i;
    for (i = 1; i < n+1; i++) {
        barrier_wait(barrier);
    if (i % 3 == 0 && i % 5 == 0) {
            (*print_fizzbuzz)();
        }
        barrier_wait(barrier2);
    }
}

void fizzbuzz_destroy() {
    barrier_destroy(barrier);
    free(barrier);
    barrier_destroy(barrier2);
    free(barrier2);
}
