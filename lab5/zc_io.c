#include "zc_io.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
// The zc_file struct is analogous to the FILE struct that you get from fopen.
struct zc_file {
  // Insert the fields you need here.
    int fd;
    size_t size;
    size_t offset;
    void *base;
};

// Reader-Writer Problem
sem_t *mutex;
sem_t *bufferEmpty;
int nReader;
/**************
 * Exercise 1 *
 **************/

zc_file *zc_open(const char *path) {
  // To implement
    struct stat buf;
    zc_file *zc;
    zc = (zc_file*) malloc(sizeof(zc_file));
    
    // Semaphore Initialization
    mutex = (sem_t*) malloc(sizeof(sem_t));
    bufferEmpty = (sem_t*) malloc(sizeof(sem_t));

    sem_init(mutex, 0, 1);
    sem_init(bufferEmpty, 0, 1);
    nReader = 0;

    //open or creates a new file
    zc->fd = open(path, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);

    // acquiring the size
    fstat(zc->fd, &buf);
    zc->size = buf.st_size;
    zc->offset = 0;

    if (zc->size != 0) {
        zc->base = mmap(NULL, zc->size, PROT_READ | PROT_WRITE, MAP_SHARED, zc->fd, 0);
    }

    if (zc->base == (void *) MAP_FAILED) {
        printf("Map Failed\n");
    }
    
    return zc;
}

int zc_close(zc_file *file) {
  // To implement
    
    sem_destroy(mutex);
    sem_destroy(bufferEmpty);
    free(mutex);
    free(bufferEmpty);

    munmap(file->base, file->size);
    close(file->fd);
    free(file);

    return -1;
}

const char *zc_read_start(zc_file *file, size_t *size) {
  // To implement
    
    sem_wait(mutex);
        nReader += 1;
        if (nReader == 1) {
            sem_wait(bufferEmpty);
        }
    sem_post(mutex);

    if (file->offset + *size > file->size) {
        *size = file->size - file->offset;
    }
    size_t prev = file->offset;
    
    file->offset += *size;
    const char *ptr =  ((char *)file->base + prev);
    
    return ptr;
}

void zc_read_end(zc_file *file) {
  // To implement
    sem_wait(mutex);
        nReader -= 1;
        if (nReader == 0) {
            sem_post(bufferEmpty);
        }
    sem_post(mutex);
}

/**************
 * Exercise 2 *
 **************/

char *zc_write_start(zc_file *file, size_t size) {
  // To implement
    sem_wait(bufferEmpty);
    size_t prev = file->offset;

    if (file->offset + size > file->size) {
        if (file->size == 0) {
            file->base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file->fd, 0);
            file->size = size;
            ftruncate(file->fd, size);
        }
        else {
            size_t new_size = file->offset + size;
            file->base = mremap(file->base, file->size, new_size, MREMAP_MAYMOVE);
            if (file->base == (void *) -1) {
                perror("Error remapping!\n");
            }
            file->size = new_size;
            ftruncate(file->fd, new_size);
        }
    }
    file->offset += size;
    char *ptr = ((char *)file->base + prev);

    return ptr;
}

void zc_write_end(zc_file *file) {
  // To implement
    msync(file->base, file->size, MS_SYNC);
    sem_post(bufferEmpty);
}

/**************
 * Exercise 3 *
 **************/

off_t zc_lseek(zc_file *file, long offset, int whence) {
  // To implement
    sem_wait(bufferEmpty);
    if (whence == 0) {
        file->offset = 0;
    } 
    if (whence == 1) {
        file->offset = file->offset;
    } 
    if (whence == 2) {
        file->offset = file->size;
    }

    file->offset += offset;

    if (file->offset < 0) {
        sem_post(bufferEmpty);
        return -1;
    }

    sem_post(bufferEmpty);
    return file->offset;
}

/**************
  Exercise 5 *
 **************/

int zc_copyfile(const char *source, const char *dest) {
  // To implement
    zc_file *src = zc_open(source);
    zc_file *des = zc_open(dest);
    
    int src_fd = src->fd;
    int des_fd = des->fd;
    size_t size = src->size;

    ssize_t result = copy_file_range(src_fd, NULL, 
                                     des_fd, NULL,
                                     size, 0);

    ftruncate(des->fd, src->size);
    
    if (result == -1) {
        return -1;
    }

    return 0;

}
