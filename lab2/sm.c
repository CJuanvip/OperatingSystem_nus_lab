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
   
// Helper function
void start_helper(const char *processes[], PID_Array* pidarr, int pipeFd[][2], int number_of_pipes, int positions[], int pid_counter) {
    int k;
    pid_t PID;

    for (k = 0; k < number_of_pipes; k++) {  
        if (k == 0 && k != number_of_pipes) { // Check if its the first process and NOT the only process

            pipe(pipeFd[k]);
                
            if ((PID = fork()) == 0) { //child process
                printf("HI CHILDREN\n");    


                close(0); //close stdin
                close(1);
                dup2(pipeFd[k][WRITE_END], STDOUT_FILENO);
                execv(processes[positions[k]], (char **) &processes[positions[k]]);
            }    
            else {
                printf("HI PARENT");    
                pidarr->arr[pid_counter] = PID;
                pid_counter += 1;
                wait(NULL);
                close(pipeFd[k][WRITE_END]);
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
   
}


// Use this function to any initialisation if you need to.
void sm_init(void) {
}

// Use this function to do any cleanup of resources.
void sm_free(void) {
}

// Exercise 1a/2: start services
void sm_start(const char *processes[]) {
    pid_t PID, w;
      
    int wstatus;
    int number_of_pipes = 0, i = 0, j = 1, pid_counter = 0;
    int positions[SM_MAX_SERVICES]; // Array of positions of processes in processes[]
    static PID_Array* pidarr; 
  
     
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

    //
    start_helper(processes, pidarr, pipeFd, number_of_pipes, positions, pid_counter);
    
            // Acquiring the path for status
            size_t len = strlen(processes[positions[j-1]]);
            char *src[len];
            *src = (char*) malloc(sizeof(char) * len);
            strcpy(*src, processes[positions[j-1]]);
            

            if ((PID = fork()) == 0) {
                close(0);
                dup2(pipeFd[j - 2][READ_END], STDIN_FILENO);   
                execv(processes[positions[j-1]], (char**) &processes[positions[j-1]]);
            }
            else {
                
                pidarr->arr[j-1]= PID;
       
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
    int length = pid_array[index]->len;
    int i, w, wstatus;

    for (i = 0; i < length; i++) {
        w = waitpid(pid_array[index]->arr[i], &wstatus, WNOHANG);
        
        if (w == 0) {
            kill(pid_array[index]->arr[i], 15);
        }
    }
}

void sm_wait(size_t index) {
    int length = pid_array[index]->len;
    int i, w, wstatus;

    for (i = 0; i < length; i++) {
        w = waitpid(pid_array[index]->arr[i], &wstatus, WNOHANG);
        
        if (w == 0) {
            kill(pid_array[index]->arr[i], 15);
        }
    }

}

void sm_shutdown(void) {
    int i;

    for (i = 0; i < SM_MAX_SERVICES; i++) {
        if (array[i] == NULL) {
            break;
        }
        else {
            sm_stop(i);       
        }
    }
}

// Exercise 4: start with output redirection
void sm_startlog(const char *processes[]) {

    
}

// Exercise 5: show log file
void sm_showlog(size_t index) {






}
