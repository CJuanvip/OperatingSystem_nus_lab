/**
 * CS2106 AY 20/21 Semester 1 - Lab 2
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include "sm.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

static sm_status_t* array[SM_MAX_SERVICES];


// Use this function to any initialisation if you need to.
void sm_init(void) {
}

// Use this function to do any cleanup of resources.
void sm_free(void) {
}

// Exercise 1a/2: start services
void sm_start(const char *processes[]) {
    pid_t PID, w;
    size_t len = strlen(processes[0]);
    char *src[len];
    *src = (char*) malloc(sizeof(char) * len);     
    int wstatus;
    PID = fork();
    strcpy(*src, processes[0]);


    if (PID == 0) { //Child Process
        execv(processes[0], (char**) processes);
    }

    else { //Parent Process
        sm_status_t* node;
        node = (sm_status_t*) malloc(sizeof(sm_status_t));
        
        w = waitpid(PID, &wstatus, WNOHANG);

        node->pid = PID;
        node->path =  *src;

        if (w == 0) {
            node->running = true;
        }

        else {
            node->running = false;
        }

        array[0] = node;        
    }

}


// Exercise 1b: print service status
size_t sm_status(sm_status_t statuses[]) {
    
    int i, count = 0;

    for (i = 0; i < SM_MAX_SERVICES; i++) {
        if (array[i] == NULL) {
            break;
        }
        else {
            statuses[i] = *array[i];
            count += 1;
        }
    }

    printf("Process: %d  Running: %d \n", statuses[0].pid, statuses[0].running);

    return (size_t) count;
}

// Exercise 3: stop service, wait on service, and shutdown
void sm_stop(size_t index) {
}

void sm_wait(size_t index) {
}

void sm_shutdown(void) {
}

// Exercise 4: start with output redirection
void sm_startlog(const char *processes[]) {
}

// Exercise 5: show log file
void sm_showlog(size_t index) {
}
