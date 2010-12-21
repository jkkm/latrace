
/* /usr/include/fcntl.h */

int fcntl(int fd, int cmd);
int open(char *file, int oflag);
int open64(char *file, int oflag);
int openat(int fd, char *file, int oflag);
int openat64(int fd, char *file, int oflag);
int creat(char *file, __mode_t mode);
int creat64(char *file, __mode_t mode);
int lockf(int fd, int cmd, __off_t len);


int lockf64(int fd, int cmd, __off64_t len);
int posix_fadvise(int fd, __off_t offset, __off_t len, int advise);
int posix_fadvise64(int fd, __off64_t offset, __off64_t len, int advise);
int posix_fallocate(int fd, __off_t offset, __off_t len);
int posix_fallocate64(int fd, __off64_t offset, __off64_t len);
