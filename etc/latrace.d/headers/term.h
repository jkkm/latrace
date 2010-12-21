
/* /usr/include/term.h */


typedef void TTY;
typedef void TERMTYPE;
typedef void TERMINAL;


int   _nc_set_tty_mode(TTY *buf);
int   _nc_get_tty_mode(TTY *buf);
int   _nc_read_entry(char *a, char *b, TERMTYPE *c);
int   _nc_read_file_entry (char *a, TERMTYPE *b);
int   _nc_read_termtype (TERMTYPE *a, char *b, int c);
char* _nc_first_name(char *a);
int   _nc_name_match(char *a, char *b, char *c);


TERMINAL*  set_curterm(TERMINAL *t);
int        del_curterm(TERMINAL *t);


int     restartterm(char *a, int b, int *c);
int     setupterm (char *a, int b, int *c);
char*   tigetstr(char *a);
int     putp(char *s);
int     tigetflag(char *a);
int     tigetnum(char *a);


char*   tparm(char *s);
char*   tparm_varargs(char *s);


char*   tgetstr(char *s, void *a);
char*   tgoto(char *s, int a, int b);
int     tgetent(char *s, char *a);
int     tgetflag(char *s);
int     tgetnum(char *s);
int     tputs(char *s, int a, void *f);
