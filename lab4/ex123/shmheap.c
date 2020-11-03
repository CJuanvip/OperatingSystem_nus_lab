/*
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
#include <semaphore.h>

shmheap_memory_handle shmheap_create(const char *name, size_t len) {
    shmheap_memory_handle heap;
    sem_t *lock;
    book *initial;
    
    // initliase the file descriptor of the heap
    heap.fd = shm_open(name, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    // truncate the length
    ftruncate(heap.fd, len);
    // set base address of the heap
    heap.base = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, heap.fd, 0);
    // set the length of the heap
    heap.len = len;
    // close the file descriptor after mmap
    close(heap.fd);
   
    // Semaphore Initialization for Ex3
    lock = (sem_t *) heap.base;
    sem_init(lock, 1, 1);
   // sem_init(sem->free, 1, 1);
    
    // Bookkeeping Initialization
    initial = (book *) ((char*)heap.base + sizeof(sem_t));
    initial->is_free = 1;
    initial->size = len - sizeof(book) - sizeof(sem_t);

    return heap;  
}

shmheap_memory_handle shmheap_connect(const char *name) {
    shmheap_memory_handle heap;
    struct stat statbuf;

    // connect to existing heap
    heap.fd = shm_open(name, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    
    // use fstat to find the length of the heap 
    fstat(heap.fd, &statbuf);
    heap.len = (size_t) statbuf.st_size;
   
    // mapping memory from heap to virutal memory of process
    void *ptr = mmap(NULL, heap.len, PROT_READ | PROT_WRITE, MAP_SHARED, heap.fd, 0);
    heap.base = ptr;
  
    close(heap.fd);
    return heap;
}

void shmheap_disconnect(shmheap_memory_handle mem) {
    munmap(mem.base, mem.len);
}

void shmheap_destroy(const char *name, shmheap_memory_handle mem) {
    sem_t *ptr;

    // destroys semaphores
    ptr = (sem_t *) mem.base;
    sem_destroy(ptr);

    munmap(mem.base, mem.len);
    shm_unlink(name);
}

void *shmheap_underlying(shmheap_memory_handle mem) {
    return mem.base;
}

void *shmheap_alloc(shmheap_memory_handle mem, size_t sz) {
    void *ptr;  // pointer to allocated slot to be returned 
    book *info;  // bookkeeping of allocated slot

    sem_t *sem = (sem_t *)mem.base;
    book *current = (book *) ((char *)mem.base + sizeof(sem_t));
    
    sem_wait(sem);
    while (1) {
//        printf("---BOOKKEEPING SLOT %d---\n", i);
//        printf("isFree: %d\n", current->is_free);
//        printf("free space: %d \n", current->size);
//        printf("size to allocate + bookkeeping: %ld\n", sz + sizeof(book));
 
        if (current->is_free == 0 || (int) (sz + sizeof(book)) > current->size) {
            current = (book *) ((char *) current + current->size + sizeof(book));
            continue;
        }
        else {
            // get the remaining space 
            int current_size = current->size;
            // sets bookkeeping for allocated slot
            current->is_free = 0;
            current->size = sz;

            //printf("bookkeeping is_free: %ld, size_allocated: %ld\n", current->is_free, current->size);
            
           // pointer to allocated space to be returned
            ptr = (void *)((char *)current + sizeof(book));

            // pointer to next bookkeeping
            info = (book *) ((char *)ptr + sz);
            info->is_free = 1;
            info->size = current_size - sz - sizeof(book); 
            
            break;
        }
    }
    sem_post(sem);

    return ptr;
}

void shmheap_free(shmheap_memory_handle mem, void *ptr) {
    sem_t *sem = (sem_t *) mem.base;
    book *loop = (book *) ((char *) mem.base + sizeof(sem_t));
    book *current = (book *)((char *)ptr - sizeof(book)); 
    book *right = (book *)((char *)ptr + current->size); //bookkeeping for next space

    sem_wait(sem);

    current->is_free = 1; //freeing the space
    
//    printf("Current is_free : %d, Current Size: %d \n", current->is_free, current->size);
   
    // handling if there's a free space after this ptr
    if (right->is_free == 1) {
        current->size += right->size + sizeof(book);
    }

    book *next;
    if (current == loop) {} //If you are removing the first slot
    else {
        while (1) {
            next = loop;
            loop = (book *) ((char *) loop + loop->size + (int) sizeof(book));
        
            if (current == loop) {
                if (next->is_free == 1) {
                    next->size += current->size + sizeof(book);
                }
            break;
            }
        }   
    }
    sem_post(sem);
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
