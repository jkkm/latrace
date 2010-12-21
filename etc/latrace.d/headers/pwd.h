
/* /usr/include/pwd.h */


struct passwd {
        char    *pw_name;
        char    *pw_passwd;
        __uid_t pw_uid;
        __gid_t pw_gid;
        char    *pw_gecos;
        char    *pw_dir;
        char    *pw_shell;
};


void setpwent();
void endpwent();
struct passwd* getpwent();
struct passwd* fgetpwent(FILE *stream);
int putpwent(struct passwd *p, FILE *f);
struct passwd* getpwuid(__uid_t uid);
struct passwd* getpwnam(char *name);


int getpwent_r(struct passwd *resultbuf, char *buffer, size_t buflen, void *result);
int getpwuid_r(__uid_t uid, struct passwd *resultbuf, char *buffer, size_t buflen, void *result);
int getpwnam_r(char *name, struct passwd *resultbuf, char *buffer, size_t buflen, void *result);
int fgetpwent_r(FILE *stream, struct passwd *resultbuf, char *buffer, size_t __buflen, void *result);
int getpw(__uid_t uid, char *buffer);
