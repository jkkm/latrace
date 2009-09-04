
/* /usr/include/stdio.h */

int remove(char *filename);
int rename(char *old, char *new);
int renameat(int oldfd, char *old, int newfd, char *new);


FILE *tmpfile(void);
FILE *tmpfile64(void);
char *tmpnam(char *s);
char *tmpnam_r(char *s);
char *tempnam(char *dir, char *pfx);


int fclose(FILE *stream);
int fflush(FILE *stream);
int fflush_unlocked(FILE *stream);
int fcloseall(void);


FILE* fopen(char *filename, char *modes);
FILE* freopen(char *filename, char *modes, FILE *stream);


FILE* fopen64(char *filename, char *modes);
FILE* freopen64(char *filename, char *modes, FILE *stream);
FILE* fdopen(int fd, char *modes);
FILE* fopencookie(void *magic_cookie, char *modes, void *io_funcs);
FILE* fmemopen(void *s, size_t len, char *modes);
FILE* open_memstream(void *bufloc, size_t *sizeloc);


void setbuf(FILE *stream, char *buf);
int  setvbuf(FILE *stream, char *buf, int modes, size_t n);
void setbuffer(FILE *stream, char *buf, size_t size);
void setlinebuf(FILE *stream);


int fprintf(FILE *stream, char *format);
int printf(char *format);
int sprintf(char *s, char *format);
int vfprintf(FILE *s, char *format);
int vprintf(char *format);
int vsprintf(char *s, char *format);
int snprintf(char *s, size_t maxlen, char *format);
int vsnprintf(char *s, size_t maxlen, char *format);
int vasprintf(void *ptr, char *f);
int __asprintf(void *ptr, char *fmt);
int asprintf(void *ptr, char *fmt);
int vdprintf(int fd, char *fmt);
int dprintf(int fd, char *fmt);
int fscanf(FILE *stream, char *format);
int scanf(char *format);
int sscanf(char *s, char *format);
int vfscanf(FILE *s, char *format);
int vscanf(char *format);
int vsscanf(char *s, char *format);

int fgetc(FILE *stream);
int getc(FILE *stream);
int getchar();
int getc_unlocked(FILE *stream);
int getchar_unlocked();
int fgetc_unlocked(FILE *__stream);
int fputc(int c, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
int fputc_unlocked(int c, FILE *stream);
int putc_unlocked(int c, FILE *stream);
int putchar_unlocked(int c);
int getw(FILE *stream);
int putw(int w, FILE *stream);
char* fgets(char *s, int n, FILE *stream);
char* gets(char *s);
char* fgets_unlocked(char *s, int n, FILE *stream);
size_t __getdelim(void *lineptr, size_t *n, int delimiter, FILE *stream);
size_t getdelim(void *lineptr, size_t *n, int delimiter, FILE *stream);
size_t getline(void *lineptr, size_t *n, FILE *stream);
int fputs(char *s, FILE *stream);
int puts(char *s);
int ungetc(int c, FILE *stream);


size_t  fread(void *ptr, size_t size, size_t n, FILE *stream);
size_t  fwrite(void *ptr, size_t size, size_t n, FILE *s);
int     fputs_unlocked(char *s, FILE *stream);
size_t  fread_unlocked(void *ptr, size_t size, size_t n, FILE *stream);
size_t  fwrite_unlocked(void *ptr, size_t size, size_t n, FILE *stream);
int     fseek(FILE *stream, long off, int whence);
long    ftell(FILE *stream);


void    rewind(FILE *stream);
int     fseeko(FILE *stream, __off_t off, int whence);
__off_t ftello(FILE *stream);
int     fgetpos(FILE *stream, fpos_t *pos);
int     fsetpos(FILE *stream, fpos_t *pos);


typedef long  __off64_t;
typedef void  fpos64_t;


int             fseeko64(FILE *stream, __off64_t off, int whence);
__off64_t       ftello64(FILE *stream);


int fgetpos64(FILE *stream, fpos64_t *pos);
int fsetpos64(FILE *stream, fpos64_t *pos);


void clearerr(FILE *stream);
int  feof(FILE *stream);
int  ferror(FILE *stream);
void clearerr_unlocked(FILE *stream);
int  feof_unlocked(FILE *stream);
int  ferror_unlocked(FILE *stream);


void perror(char *s);


int     fileno(FILE *stream);
int     fileno_unlocked(FILE *stream);
FILE*   popen(char *command, char *modes);
int     pclose(FILE *stream);
char*   ctermid(char *s);
char*   cuserid(char *s);


int  obstack_printf(void *obstack, char *format);
int  obstack_vprintf(void *obstack, char *format);
void flockfile(FILE *stream);
int  ftrylockfile(FILE *stream);
void funlockfile(FILE *stream);

