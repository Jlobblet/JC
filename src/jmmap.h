/// \file
/// Convenience functions for memory-mapped files.
#ifndef JC_JMMAP_H
#define JC_JMMAP_H

#include <sys/stat.h>

/// Struct containing data about a memory-mapped file.
typedef struct jc_mmap {
    /// Pointer to the start of the file.
    volatile char* address;
    /// File descriptor.
    int fd;
    /// Results of fstat.
    struct stat stat;
} jc_mmap;

iptr mmap_read(const char* filepath, jc_mmap* buf);
iptr mmap_close(jc_mmap* buf);

#endif //JC_JMMAP_H
