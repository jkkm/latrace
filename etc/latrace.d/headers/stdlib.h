
/* /usr/include/stdlib.h */

size_t __ctype_get_mb_cur_max(void);

int   atoi(char *nptr);
long  atol(char *nptr);
long  atoll(char *nptr);
long  strtol(char *nptr, void *endptr, int base);


/*      we dont do big numbers so far...

double                  atof(char *nptr);
double                  strtod(char *nptr, void *endptr);
float                   strtof(char *nptr, void *endptr);
long double             strtold(char *nptr, void *endptr);
u_long                  strtoul(char *nptr, void *endptr, int base);
long long int           strtoq(char *nptr, void *endptr, int base);
unsigned long long int  strtouq(char *nptr, void *endptr, int base);
long long int           strtoll(char *nptr, void *endptr, int base);
unsigned long long int  strtoull(char *nptr, void *endptr, int base);
long                    strtol_l(char *nptr, void *endptr, int base, locale_t loc);
unsigned long int       strtoul_l(char *nptr, void *endptr, int base, locale_t loc);
long long int           strtoll_l(char *nptr, void *endptr, int base, locale_t loc);
unsigned long long int  strtoull_l(char *nptr, void *endptr, int base, locale_t loc);
double                  strtod_l(char *nptr, void *endptr, locale_t loc);
float                   strtof_l(char *nptr, void *endptr, locale_t loc);
long double             strtold_l(char *nptr, void *endptr, locale_t loc);

*/


char* l64a(long n);
long  a64l(char *s);


long    random(void);
void    srandom(u_int seed);
char*   initstate(u_int seed, char* statebuf, size_t statelen);
char*   setstate(char *statebuf);


struct random_data
{
        int32_t *fptr;
        int32_t *rptr;
        int32_t *state;
        int rand_type;
        int rand_deg;
        int rand_sep;
        int32_t *end_ptr;
};

int     random_r(struct random_data *buf, int32_t *result);
int     srandom_r(u_int seed, struct random_data *buf);
int     initstate_r(u_int seed, char *statebuf, size_t statelen, struct random_data *buf);
int     setstate_r(char *statebuf, struct random_data *buf);
int     rand(void);
void    srand(u_int seed);
int     rand_r(u_int *seed);


/*      we dont do big numbers so far...

double drand48(void);
double erand48(void *ptr);

*/


long    lrand48(void);
long    nrand48(void *xsubi);

long    mrand48(void);
long    jrand48(void *xsubi);

void    srand48(long seedval);
u_int*  seed48 (void *seed16v);

void    lcong48(void *param);


/*      no struct arrays so far...

struct drand48_data
{
        unsigned short int x[3];
        unsigned short int old_x[3];
        unsigned short int c;
        unsigned short int init;
        unsigned long long int a;
};

int drand48_r(struct drand48_data *restrict buffer, double *result);
int erand48_r(unsigned short int xsubi[3], struct drand48_data *restrict buffer, double *result);
int lrand48_r(struct drand48_data *restrict buffer, long int *restrict result);
int nrand48_r(unsigned short int xsubi[3], struct drand48_data *restrict buffer, long int *restrict result);
int mrand48_r(struct drand48_data *restrict buffer, long int *restrict result);
int jrand48_r(unsigned short int xsubi[3], struct drand48_data *restrict buffer, long int *restrict result);
int srand48_r(long int seedval, struct drand48_data *buffer);
int seed48_r(unsigned short int seed16v[3], struct drand48_data *buffer);
int lcong48_r(unsigned short int param[7], struct drand48_data *buffer);

*/


void*   malloc(size_t size);
void*   calloc(size_t nmemb, size_t size);
void*   realloc(void *ptr, size_t size);
void    free(void *ptr);
void    cfree(void *ptr);
void*   valloc(size_t size);


int     posix_memalign(void *memptr, size_t alignment, size_t size);
void    abort(void);
int     atexit(void *func);
int     on_exit(void *func, void *arg);
void    exit(int status);
void    _Exit(int status);
char*   getenv(char *name);
char*   secure_getenv (char *name);
int     putenv(char *string);
int     setenv(char *name, char *value, int replace);
int     unsetenv(char *name);
int     clearenv(void);


char*   mktemp(char *template);
int     mkstemp(char *template);
int     mkstemp64(char *template);
char*   mkdtemp(char *template);
int     mkostemp(char *template, int flags);
int     mkostemp64(char *template, int flags);


int     system(char *command);
char*   canonicalize_file_name(char *name);
char*   realpath(char *name, char *resolved);


void*   bsearch(void *key, void *base, size_t nmemb, size_t size, void *compar);
void    qsort(void *base, size_t nmemb, size_t size, void *compar);


int     abs(int x);
long    labs(long x);


struct div_t
{
        int quot;
        int rem;
};

struct ldiv_t
{
        long quot;
        long rem;
};

struct div_t div(int numer, int denom);
struct ldiv_t ldiv(long numer, long denom);


/*      we dont do big numbers so far...

long long int llabs (long long int x);

struct lldiv_t {
        long long int quot;
        long long int rem;
};

lldiv_t lldiv(long long int numer, long long int denom);
char*   ecvt(double value, int ndigit, int *decpt, int *sign);
char*   fcvt(double value, int ndigit, int *decpt, int *sign);
char*   gcvt(double value, int ndigit, char *buf);
char*   qecvt(long double value, int ndigit, int *decpt, int *sign);
char*   qfcvt(long double value, int ndigit, int *decpt, int *sign);
char*   qgcvt(long double value, int ndigit, char *buf);
int     ecvt_r(double value, int ndigit, int *decpt, int *sign, char *buf, size_t len);
int     fcvt_r(double value, int ndigit, int *decpt, int *sign, char *buf, size_t len);
int     qecvt_r(long double value, int ndigit, int *decpt, int *sign, char *buf, size_t len);
int     qfcvt_r(long double value, int ndigit, int *decpt, int *sign, char *buf, size_t len);

*/


typedef char wchar_t;

int     mblen(char *s, size_t n);
int     mbtowc(wchar_t *pwc, char *s, size_t n);
int     wctomb(char *s, wchar_t wchar);
size_t  mbstowcs(wchar_t *pwcs, char *s, size_t n);
size_t  wcstombs(char *s, wchar_t *pwcs, size_t n);


int     rpmatch(char *response);
int     getsubopt(void *optionp, void *tokens, void *valuep);
void    setkey(char *key);
int     posix_openpt(int oflag);
int     grantpt(int fd);
int     unlockpt(int fd);
char*   ptsname(int fd);
int     ptsname_r(int fd, char *buf, size_t buflen);
int     getpt(void);
int     getloadavg(void *ptr, int nelem);
void    __cxa_finalize(void *ptr);
