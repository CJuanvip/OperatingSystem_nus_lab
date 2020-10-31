/*************************************
* Lab 4
* Name: Kieron Koh
* Student No: A0206059H
* Lab Group: 13
*************************************/

#include "shmheap.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

shmheap_memory_handle shmheap_create(const char *name, size_t len) {
    shmheap_memory_handle heap;
    
    // initliase the file descriptor of the heap
    heap.fd = shm_open(name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (heap.fd != 0) {
        perror("ERROR");
    }
    // truncate the length
    ftruncate(heap.fd, len);
    // set base address of the heap
    heap.base = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, heap.fd, 0);
   // heap.base = ptr;
    // set the length of the heap
    heap.len = len;
    // close the file descriptor after mmap
    close(heap.fd);
    
    printf("heap created");
    return heap;
  
}

shmheap_memory_handle shmheap_connect(const char *name) {



    shmheap_memory_handle heap;
    struct stat statbuf;

    heap.fd = shm_open(name, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    // use fstat to find the length of the heap 
    fstat(heap.fd, &statbuf);

    heap.len = (size_t) statbuf.st_size;
   
    void *ptr = mmap(NULL, heap.len, PROT_READ | PROT_WRITE, MAP_SHARED, heap.fd, 0);
    heap.base = ptr;
    close(heap.fd);
    return heap;
    
}

void shmheap_disconnect(shmheap_memory_handle mem) {
    munmap(mem.base, mem.len);
}

void shmheap_destroy(const char *name, shmheap_memory_handle mem) {
    munmap(mem.base, mem.len);
    shm_unlink(name);
}

void *shmheap_underlying(shmheap_memory_handle mem) {
    /* TODO */
}

void *shmheap_alloc(shmheap_memory_handle mem, size_t sz) {
  //  void *ptr;
    
    return mem.base;
}

void shmheap_free(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
}

shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr) {
    shmheap_object_handle hdl;
    
    hdl.offset = ptr - mem.base;    

    return hdl;
}

void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl) {
    void *ptr;

    ptr = mem.base + hdl.offset;

    return ptr;
}
