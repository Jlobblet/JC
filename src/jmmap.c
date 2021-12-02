#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "jint.h"
#include "jmmap.h"

iptr mmap_read(const char* filepath, jc_mmap* buf) {
    buf->fd = open(filepath, O_RDONLY);
    if (buf->fd == -1) {
        return -1;
    }

    if (fstat(buf->fd, &buf->stat) == -1) {
        close(buf->fd);
        return -1;
    }

    iptr size = buf->stat.st_size;
    buf->address = mmap(NULL, size, PROT_READ, MAP_PRIVATE, buf->fd, 0);
    if (buf->address == MAP_FAILED) {
        close(buf->fd);
        return -1;
    }
    return size;
}

iptr mmap_close(jc_mmap* buf) {
    return munmap(buf->address, buf->stat.st_size)
           || close(buf->fd)
           || 0;
}
