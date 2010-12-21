
/* /usr/include/utmp.h */


typedef void utmp;


int  login_tty(int fd);
void login(utmp *entry);
int  logout(char *ut_line);


void logwtmp(char *ut_line, char *ut_name, char *ut_host);
void updwtmp(char *wtmp_file, utmp *utmp);


int utmpname(char *file);


utmp* getutent();
void  setutent();
void  endutent();
utmp* getutid(utmp *id);


utmp* getutline(utmp *line);
utmp* pututline(utmp *utmp_ptr);
int   getutent_r(utmp *buffer, void *result);
int   getutid_r(utmp *id, utmp *buffer, void *result);
int   getutline_r(utmp *line, utmp *buffer, void *result);
