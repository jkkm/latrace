
/* /usr/include/unistd.h */


int access(char *name, int type);
int euidaccess(char *name, int type);
int eaccess(char *name, int type);
int faccessat(int fd, char *file, int type, int flag);


__off_t   lseek(int fd, __off_t offset, int whence);
__off64_t lseek64(int fd, __off64_t offset, int whence);


int    close(int fd);
size_t read(int fd, void *buf, size_t nbytes);
size_t write(int fd, void *buf, size_t n);
size_t pread(int fd, void *buf, size_t nbytes, __off_t offset);
size_t pwrite(int fd, void *buf, size_t n, __off_t offset);
size_t pread64(int fd, void *buf, size_t nbytes, __off64_t offset);
size_t pwrite64(int fd, void *buf, size_t n, __off64_t offset);


int   pipe(void *pipe);
u_int alarm(u_int seconds);
u_int sleep(u_int seconds);
u_int ualarm(u_int __value, u_int interval);
int   usleep(u_int useconds);
int   pause();


int   chown(char *file, __uid_t owner, __gid_t group);
int   fchown(int fd, __uid_t owner, __gid_t group);
int   lchown(char *file, __uid_t owner, __gid_t group);
int   fchownat(int fd, char *file, __uid_t owner, __gid_t group, int flag);
int   chdir(char *path);
int   fchdir(int fd);
char* getcwd(char *buf, size_t size);


char* get_current_dir_name();
char* getwd(char *buf);


int dup(int fd);
int dup2(int fd, int fd2);


int execve(char *path, void *argv, void *envp);
int fexecve(int fd, void *argv, void *envp);
int execv(char *path, void *argv);
int execle(char *path, char *arg);
int execl(char *path, char *arg);
int execvp(char *file, void *argv);
int execlp(char *file, char *arg);


int    nice(int inc);
void   _exit(int status);
long   pathconf(char *path, int name);
long   fpathconf(int fd, int name);
long   sysconf(int name);
size_t confstr(int name, char *buf, size_t len);


__pid_t getpid();
__pid_t getppid();
__pid_t getpgrp();
__pid_t __getpgid(__pid_t pid);
__pid_t getpgid(__pid_t pid);
int     setpgid(__pid_t pid, __pid_t pgid);
int     setpgrp();
__pid_t setsid();
__pid_t getsid(__pid_t pid);
__uid_t getuid();
__uid_t geteuid();
__gid_t getgid();
__gid_t getegid();


int getgroups(int size, void *list);
int group_member(__gid_t gid);
int setuid(__uid_t uid);
int setreuid(__uid_t ruid, __uid_t euid);
int seteuid(__uid_t uid);
int setgid(__gid_t gid);
int setregid(__gid_t rgid, __gid_t egid);
int setegid(__gid_t gid);
int getresuid(__uid_t *ruid, __uid_t *euid, __uid_t *suid);
int getresgid(__gid_t *rgid, __gid_t *egid, __gid_t *sgid);
int setresuid(__uid_t ruid, __uid_t euid, __uid_t suid);
int setresgid(__gid_t rgid, __gid_t egid, __gid_t sgid);


__pid_t fork();
__pid_t vfork();


char* ttyname(int fd);
int ttyname_r(int fd, char *buf, size_t buflen);
int isatty(int fd);
int ttyslot();


int     link(char *from, char *to);
int     linkat(int fromfd, char *from, int tofd, char *to, int flags);
int     symlink(char *from, char *to);
size_t  readlink(char *path, char *buf, size_t len);
int     symlinkat(char *from, int tofd, char *to);
size_t  readlinkat(int fd, char *path, char *buf, size_t len);
int     unlink(char *name);
int     unlinkat(int fd, char *name, int flag);
int     rmdir(char *path);


__pid_t tcgetpgrp(int fd);
int     tcsetpgrp(int fd, __pid_t pgrp_id);
char*   getlogin();
int     getlogin_r(char *name, size_t name_len);
int     setlogin(char *name);


int gethostname(char *name, size_t len);
int sethostname(char *name, size_t len);
int sethostid(long id);
int getdomainname(char *name, size_t len);
int setdomainname(char *name, size_t len);


int vhangup();
int revoke(char *file);
int profil(u_short *sample_buffer, size_t __size, size_t offset, u_int scale);
int acct(char *name);


char *getusershell();
void endusershell();
void setusershell();


int   daemon(int nochdir, int noclose);
int   chroot(char *path);
char* getpass(char *prompt);
int   fsync(int fd);
long  gethostid();
void  sync();
int   getpagesize();
int   getdtablesize();


int truncate(char *file, __off_t length);
int truncate64(char *file, __off64_t length);
int ftruncate(int fd, __off_t length);
int ftruncate64(int fd, __off64_t length);


int   brk(void *addr);
void* sbrk(u_int delta);
long  syscall(long sysno = SYSCALL_NO);

int   lockf(int fd, int cmd, __off_t len);
int   lockf64(int fd, int cmd, __off64_t len);
int   fdatasync(int fildes);


char* crypt(char *key, char *salt);
void  encrypt(char *block, int edflag);

void  swab(void *from, void *to, size_t n);
char* ctermid(char* s);
