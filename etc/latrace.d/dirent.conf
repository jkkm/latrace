
/* /usr/include/dirent.h */


DIR* opendir(char *name);
DIR* fdopendir(int fd);
int  closedir(DIR *dirp);


void* readdir(DIR *dirp);
void* readdir64(DIR *dirp);
int   readdir_r(DIR *dirp, void *entry, void *result);
int   readdir64_r(DIR *dirp, void *entry, void *result);


void rewinddir(DIR *dirp);
void seekdir(DIR *dirp, long __pos);
long telldir(DIR *dirp);
int  dirfd(DIR *dirp);


int scandir(char *dir, void *namelist, void *selector, void *cmp);
int scandir64(char *dir, void *namelist, void *selector, void *cmp);
int alphasort(void *e1, void *e2);
int alphasort64(void *e1, void *e2);
int versionsort(void *e1, void *e2);
int versionsort64(void *e1, void *e2);


size_t getdirentries(int fd, char *buf, size_t nbytes, __off_t *basep);
size_t getdirentries64(int fd, char *buf, size_t nbytes, __off64_t *basep);
