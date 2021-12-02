#ifndef JC_JMMAP_H
#define JC_JMMAP_H

#include <sys/stat.h>

typedef struct jc_mmap {
    char* address;
    int fd;
    struct stat stat;
} jc_mmap;

iptr mmap_read(const char* filepath, jc_mmap* buf);
iptr mmap_close(jc_mmap* buf);

#endif //JC_JMMAP_H
