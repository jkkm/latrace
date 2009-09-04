
/* /usr/include/dlfcn.h */

void* dlopen(char *file, int mode);
int   dlclose(void *handle);
void* dlsym(void *handle, char* name);
void* dlmopen(long nsid, char *file, int mode);
void* dlvsym(void *handle, char *name, char* version);
char* dlerror();
int   dladdr(void *address, void *info);
int   dladdr1(void *address, void *info, void *extra_info, int flags);
int   dlinfo(void *handle, int request, void *arg);
