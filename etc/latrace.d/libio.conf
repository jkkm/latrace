
/* /usr/include/libio.h  UNCOMPLETE */


typedef void _IO_FILE;
typedef size_t _IO_size_t;


int __underflow(_IO_FILE *f);
int __uflow(_IO_FILE *f);
int __overflow(_IO_FILE *f, int a);


int _IO_getc(_IO_FILE *fp);
int _IO_putc(int c, _IO_FILE *fp);
int _IO_feof(_IO_FILE *fp);
int _IO_ferror(_IO_FILE *fp);


int  _IO_peekc_locked(_IO_FILE *fp);
void _IO_flockfile(_IO_FILE *f);
void _IO_funlockfile(_IO_FILE *f);
int  _IO_ftrylockfile(_IO_FILE *f);


int _IO_vfscanf(_IO_FILE *f, char *s);
int _IO_vfprintf(_IO_FILE *f, char *s);
_IO_size_t _IO_padn(_IO_FILE *f, int b, _IO_size_t s);
_IO_size_t _IO_sgetn(_IO_FILE *f, void *p, _IO_size_t s);
