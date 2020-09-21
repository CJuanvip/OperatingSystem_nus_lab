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


#define READ_END 0
#define WRITE_END 1


typedef struct vectord {
    int len;    // there must be at least one other data member
    pid_t arr[]; // the flexible array member must be last
} PID_Array;


static PID_Array* pid_array[SM_MAX_SERVICES];
static sm_status_t* array[SM_MAX_SERVICES];
static int counter = 0;


// Use this function to any initialisation if you need to.
void sm_init(void) {
}

// Use this function to do any cleanup of resources.
void sm_free(void) {
}

// Exercise 1a/2: start services
void sm_start(const char *processes[]) {
    pid_t PID, w;
      
    int wstatus, k, pid_counter = 0, number_of_pipes = 0, i = 0, j = 1;
    int positions[SM_MAX_SERVICES]; // Array of positions of processes in processes[]
    PID_Array* pidarr; 
  
   
    positions[0] = 0;
    // Getting the posiition of the paths in processes[] and the number of pipes needed
    while (true) { 
        if (processes[i] == NULL) {
            if (processes[i+1] == NULL) {
                break;     
            }
            else {
                positions[j] = i+1;
                j += 1;
            }
            number_of_pipes += 1;
        }       
        i += 1;
    }


    pidarr = malloc(sizeof(PID_Array) + j * sizeof(pid_t));
    pidarr->len = j;
    int pipeFd[number_of_pipes][2];  

    for (k = 0; k < number_of_pipes + 1; k++) { 

        if (k == 0 && k != number_of_pipes) { // At the beginning of the Service
            
            pipe(pipeFd[k]);

            if ((PID = fork()) == 0) { //child process
                close(0); //close stdin
                close(1);
                dup2(pipeFd[k][WRITE_END], STDOUT_FILENO);
                execv(processes[positions[k]], (char **) &processes[positions[k]]);
            }    
            else {
                pidarr->arr[pid_counter] = PID;
                pid_counter += 1;
                wait(NULL);
                close(pipeFd[k][WRITE_END]);
            }
        }

        else if (k == number_of_pipes) { // At the end of the Service OR only one process in a service
            // Acquiring the path for status
            size_t len = strlen(processes[positions[k]]);
            char *src[len];
            *src = (char*) malloc(sizeof(char) * len);
            strcpy(*src, processes[positions[k]]);
            

            if ((PID = fork()) == 0) {
                close(0);
                dup2(pipeFd[k - 1][READ_END], STDIN_FILENO);   
                execv(processes[positions[k]], (char**) &processes[positions[k]]);
            }
            else {

                pidarr->arr[pid_counter] = PID;
       
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
                
                pid_array[counter] = pidarr;
                array[counter] = node;
                counter += 1;
           }
        }

        else {
            pipe(pipeFd[k]);

            if ((PID = fork()) == 0) { //child process
                close(0);
                close(1);
                dup2(pipeFd[k-1][READ_END], STDIN_FILENO);
                dup2(pipeFd[k][WRITE_END], STDOUT_FILENO);
                
                //Right now, both ends of pipe is open
                execv(processes[positions[k]], (char**) &processes[positions[k]]);
            }
            else {
                pidarr->arr[pid_counter] = PID;
                pid_counter += 1;
                wait(NULL);
                close(pipeFd[k][WRITE_END]);
                close(pipeFd[k-1][READ_END]);
            }
        }
    
    }

    int s;
    printf("LIST OF PROCESSES AND THEIR PIDS\n");
    for (s = 0; s < j; s++) {
        printf("Process: %s     PID:  %d\n", processes[positions[s]], pidarr->arr[s]);
    }
    
// QUESTION 1a
//    PID = fork();
//   // strcpy(*src, processes[0]);
//
//
//    if (PID == 0) { //Child Process
//        execv(processes[0], (char**) processes);
//    }
//
//    else { //Parent Process
//        sm_status_t* node;
//        node = (sm_status_t*) malloc(sizeof(sm_status_t));
//        
//        w = waitpid(PID, &wstatus, WNOHANG);
//
//        node->pid = PID;
//        node->path =  *src;
//
//        if (w == 0) {
//            node->running = true;
//        }
//
//        else {
//            node->running = false;
//        }
//
//        array[counter] = node;
//        counter += 1;
//    }
//
}


// Exercise 1b: print service status
size_t sm_status(sm_status_t statuses[]) {
    
   int i, status;

   for (i = 0; i < SM_MAX_SERVICES; i++) {
       if (array[i] == NULL) {
           break;
       }
       else {
           pid_t return_pid = waitpid(array[i]->pid, &status, WNOHANG);
           if (return_pid != 0) {
                array[i]->running = false;
           }
           statuses[i] = *array[i];
       }
   }

   // printf("Process: %d  Running: %d \n", statuses[0].pid, statuses[0].running);

    return (size_t) counter;
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
