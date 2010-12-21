
/* /usr/include/sys/mman.h */

extern void *mmap(void *addr, size_t len, int prot,
                   int flags, int fd, __off_t offset);

extern void *mmap64(void *addr, size_t len, int prot,
                     int flags, int fd, long offset);

extern int munmap(void *addr, size_t len);
extern int mprotect(void *addr, size_t len, int prot);
extern int msync(void *addr, size_t len, int flags);
extern int posix_madvise(void *addr, size_t len, int advice);
extern int mlock(void *addr, size_t len);
extern int munlock(void *addr, size_t len);
extern int mlockall(int flags);
extern int munlockall(void);
extern int mincore(void *start, size_t len, u_char *vec);

extern void *mremap(void *addr, size_t old_len, size_t new_len,
                     int flags);
extern int remap_file_pages(void *start, size_t size, int prot,
                             size_t pgoff, int flags);
extern int shm_open(char *name, int oflag, mode_t mode);
extern int shm_unlink(char *name);
