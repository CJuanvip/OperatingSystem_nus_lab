/*************************************
* Lab 4
* Name: Kieron Koh
* Student No: A0206059H
* Lab Group: 13
*************************************/

#include "mmf.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

void *mmf_create_or_open(const char *name, size_t sz) {
    int fd;

    fd = open(name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == -1) {
         perror("error creating file\n");
               exit(1);
    }
    
    int n = ftruncate(fd, sz);
    int *ptr = mmap(NULL, sz, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    
    return ptr;
}

void mmf_close(void *ptr, size_t sz) {
    munmap(ptr, sz);
}
