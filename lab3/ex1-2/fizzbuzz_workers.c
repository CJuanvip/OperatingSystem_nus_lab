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
static int counter;
barrier_t *barrier;
barrier_t *barrier2;
sem_t *sem;
sem_t *lock;

void fizzbuzz_init ( int n ) {
    barrier = malloc(sizeof(barrier_t)); 
    barrier_init(barrier, 4);
    barrier2 = malloc(sizeof(barrier_t)); 
    barrier_init(barrier2, 4);


}

void num_thread( int n, void (*print_num)(int) ) {
    int i;
    for (i = 1; i < n+1; i++) {
    //    printf("num thread before entering\n");
        barrier_wait(barrier);
    //    printf("num thread before exiting\n");
   
        if (i % 3 != 0 && i % 5 != 0) {
            (*print_num)(i);
        }
      //  printf("num thread after entering\n");
        barrier_wait(barrier2);
      //  printf("num thread after exiting\n");
    }
}

void fizz_thread( int n, void (*print_fizz)(void) ) {
   int i;
    for (i = 1; i < n+1; i++) {
  //    printf("fizz thread before entering\n");
        barrier_wait(barrier);
    //    printf("fizz thread before exiting\n");
          if (i % 3 == 0 && i % 5 != 0) {
      //      printf("hello");
            (*print_fizz)();
        }
    //    printf("fizz thread after entering\n");
           barrier_wait(barrier2);
    //    printf("fizz thread after exiting\n");
    }
}

void buzz_thread( int n, void (*print_buzz)(void) ) {
    int i;
    for (i = 1; i < n+1; i++) {
    //       printf("buzz thread before entering\n");
        barrier_wait(barrier);
      //  printf("buzz thread before exiting\n");
    if (i % 3 != 0 && i % 5 == 0) {
            (*print_buzz)();
        }
     //    printf("buzz thread after entering\n");
           barrier_wait(barrier2);
     //   printf("buzz thread after exiting\n");
    } 
}

void fizzbuzz_thread( int n, void (*print_fizzbuzz)(void) ) {
    int i;
    for (i = 1; i < n+1; i++) {
    //       printf("fizzbuzz thread before entering\n");
        barrier_wait(barrier);
    //    printf("fizzbuzz thread before exiting\n");
    if (i % 3 == 0 && i % 5 == 0) {
            (*print_fizzbuzz)();
        }
  //       printf("fizzbuzz thread after entering\n");
           barrier_wait(barrier2);
    //    printf("fizzbuzz thread after exiting\n");
    }
}



void fizzbuzz_destroy() {
    barrier_destroy(barrier);
    free(barrier);
    barrier_destroy(barrier2);
    free(barrier2);
}
