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


shmheap_memory_handle shmheap_create(const char *name, size_t len) {
    shmheap_memory_handle heap;
    
    heap.name = name;
    heap.fd = shm_open(name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO:);
    
    ftruncate(heap.fd, len);
    mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, heap.fd, 0);
    close(heap.fd);
    
    return heap;
}

shmheap_memory_handle shmheap_connect(const char *name) {
    shmheap_memory_handle heap;
    
    heap.name = name;
    heap.fd = shm_open(name, O_RDWR);

    return heap;
}

void shmheap_disconnect(shmheap_memory_handle mem) {
    munmap(mem.name); 
}

void shmheap_destroy(const char *name, shmheap_memory_handle mem) {
    /* TODO */
    munmap
}

void *shmheap_underlying(shmheap_memory_handle mem) {
    /* TODO */
}

void *shmheap_alloc(shmheap_memory_handle mem, size_t sz) {
    /* TODO */
}

void shmheap_free(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
}

shmheap_object_handle shmheap_ptr_to_handle(shmheap_memory_handle mem, void *ptr) {
    /* TODO */
}

void *shmheap_handle_to_ptr(shmheap_memory_handle mem, shmheap_object_handle hdl) {
    /* TODO */
}
