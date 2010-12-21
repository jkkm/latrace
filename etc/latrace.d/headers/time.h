
/* /usr/include/time.h */


typedef u_int time_t;
typedef u_int clockid_t;
typedef void tm;
typedef void timespec;
typedef void timer_t;
typedef void itimerspec;


time_t time(time_t *timer);


double difftime(time_t time1, time_t time0);


time_t mktime(tm *tp);


size_t strftime(char *s, size_t maxsize, char *format, tm *tp);
char*  strptime(char *s, char *fmt, tm *tp);
size_t strftime_l(char *s, size_t maxsize, char *format, tm *tp, __locale_t __loc);
char*  strptime_l(char *s, char *fmt, tm *tp, __locale_t __loc);


tm*    gmtime(time_t *timer);
tm*    localtime(time_t *timer);
tm*    gmtime_r(time_t *timer, tm *tp);
tm*    localtime_r(time_t *timer, tm *tp);


char*  asctime(tm *tp);
char*  ctime(time_t *timer);
char*  asctime_r(tm *tp, char *buf);
char*  ctime_r(time_t *timer, char *buf);


void tzset();
int dysize(int year);
int nanosleep(timespec *requested_time, timespec *remaining);


int    stime(time_t *when);
time_t timegm(tm *tp);
time_t timelocal(tm *tp);


int clock_getres(clockid_t clock_id, timespec *res);
int clock_gettime(clockid_t clock_id, timespec *tp);
int clock_settime(clockid_t clock_id, timespec *tp);
int clock_nanosleep(clockid_t clock_id, int flags, timespec *req, timespec *rem);
int clock_getcpuclockid(pid_t pid, clockid_t *clock_id);


int timer_create(clockid_t clock_id, void *evp, timer_t *timerid);
int timer_delete(timer_t *timerid);
int timer_settime(timer_t timerid, int flags, itimerspec *value, itimerspec *ovalue);
int timer_gettime(timer_t timerid, itimerspec *value);
int timer_getoverrun(timer_t timerid);


tm* getdate(char *string);
int getdate_r(char *string, tm *resbufp);
