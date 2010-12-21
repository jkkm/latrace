
/* /usr/include/sys/stat.h */


typedef void stat;
typedef void stat64;

int stat(char *file, stat *buf);
int fstat(int fd, stat *buf);
int stat64(char *file, stat64 *buf);
int fstat64(int fd, stat64 *buf);
int fstatat(int fd, char *file, stat *buf, int flag);
int fstatat64(int fd, char *file, stat64 *buf, int flag);
int lstat(char *file, stat *buf);
int lstat64(char *file, stat64 *buf);


int chmod(char *file, __mode_t mode);
int lchmod(char *file, __mode_t mode);
int fchmod(int fd, __mode_t mode);
int fchmodat(int fd, char *file, __mode_t mode, int flag);


__mode_t umask(__mode_t mask);
__mode_t getumask();


int mkdir(char *path, __mode_t mode);
int mkdirat(int fd, char *path, __mode_t mode);

int mknod(char *path, __mode_t mode, __dev_t dev);
int mknodat(int fd, char *path, __mode_t mode, __dev_t dev);


int mkfifo(char *path, __mode_t mode);
int mkfifoat(int fd, char *path, __mode_t mode);


int utimensat(int fd, char *path, void *times, int flags);
int futimens(int fd, void *times);


int __fxstat(int ver, int fildes, stat *stat_buf);
int __xstat(int ver, char *filename, stat *stat_buf);
int __lxstat(int ver, char *filename, stat *stat_buf);
int __fxstatat(int ver, int fildes, char *filename, stat *stat_buf, int flag);
int __fxstat64(int ver, int fildes, stat64 *stat_buf);
int __xstat64(int ver, char *filename, stat64 *stat_buf);
int __lxstat64(int ver, char *filename, stat64 *stat_buf);
int __fxstatat64(int ver, int fildes, char *filename, stat64 *stat_buf, int flag);


int __xmknod(int ver, char *path, __mode_t mode, __dev_t *dev);
int __xmknodat(int ver, int fd, char *path, __mode_t mode, __dev_t *dev);
