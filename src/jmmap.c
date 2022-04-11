/// \file
/// Convenience functions for memory-mapped files.
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include "jint.h"
#include "jmmap.h"

/// Memory map a file for reading only.
/// \param filepath Path to the file to open.
/// \param buf jc_mmap to store mapped file information in.
/// \return On success, return the size of the mapped file in bytes (a non-negative integer).
/// On error, -1 is returned and errno is set to indicate the error.
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

/// Close a memory-mapped jc_mmap file.
/// \param buf The jc_mmap to close.
/// \return On success, 0 is returned.
/// On failure, -1 is returned and errno is set to indicate the error.
iptr mmap_close(jc_mmap* buf) {
    return munmap(buf->address, buf->stat.st_size)
           || close(buf->fd)
           || 0;
}

/// Memory map a file for writing.
/// \param Filepath Path to the file to open.
/// \param buf jc_mmap to store mapped file information in.
/// \return On success, return the size of the mapped file in bytes (a non-negative integer).
/// On error, -1 is returned and errno is set to indicate the error.
/// \note The file will be created if it does not exist.
/// \note The file will be opened in read/write mode.
iptr mmap_read_write(const char* filepath, jc_mmap* buf) {
    buf->fd = open(filepath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (buf->fd == -1) {
        return -1;
    }

    if (fstat(buf->fd, &buf->stat) == -1) {
        close(buf->fd);
        return -1;
    }

    iptr size = buf->stat.st_size;
    buf->address = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, buf->fd, 0);
    if (buf->address == MAP_FAILED) {
        close(buf->fd);
        return -1;
    }
    return size;
}
