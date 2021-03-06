
/* /usr/include/ncurses.h */

typedef int     attr_t;
typedef attr_t  chtype;
typedef u_char  bool;

bool    is_term_resized(int a, int b);
char*   keybound (int a, int b);
char*   curses_version(void);
int     assume_default_colors(int a, int b);
int     define_key(char *a, int b);
int     key_defined(char *a);
int     keyok(int a, int b);
int     resize_term(int a, int b);
int     resizeterm(int a, int b);
int     use_default_colors(void);
int     use_extended_names(int a);
int     use_legacy_coding(int a);
int     wresize (void *w, int a, int b);
void    nofilter(void);


int wgetch_events(void *w, void *event);
int wgetnstr_events(void *w, char *a, int b, void *event);

int     addch(chtype a);
int     addchnstr(chtype *a, int b);
int     addchstr(chtype *a);
int     addnstr(char *a, int b);
int     addstr(char *a);
int     attroff(attr_t a);
int     attron(attr_t a);
int     attrset(attr_t a);
int     attr_get(attr_t *a, short *b, void *c);
int     attr_off(attr_t a, void *b);
int     attr_on(attr_t a, void *b);
int     attr_set(attr_t a, short b, void *c);
int     baudrate();
int     beep();
int     bkgd(chtype a);
void    bkgdset(chtype a);
int     border(chtype a, chtype b, chtype c, chtype d, chtype c, chtype d, chtype e, chtype f);
int     box(void *w, chtype a, chtype b);
bool    can_change_color();
int     cbreak(void);
int     chgat(int a, attr_t b, short c, void *d);
int     clear();
int     clearok(void *w, bool a);
int     clrtobot(void);
int     clrtoeol(void);
int     color_content(short a, short *b, short *c, short *d);
int     color_set(short a, void *b);
int     COLOR_PAIR(int a);
int     copywin(void *w1, void *w2, int a, int b, int c, int d, int e, int f, int g);
int     curs_set(int a);
int     def_prog_mode(void);
int     def_shell_mode(void);
int     delay_output(int a);
int     delch(void);
void    delscreen(void *s);
int     delwin(void *w);
int     deleteln();
void*   derwin(void *w, int a, int b, int c, int d);
int     doupdate();
void*   dupwin(void *w);
int     echo();
int     echochar(chtype a);
int     erase();
int     endwin();
char    erasechar();
void    filter();
int     flash();
int     flushinp();
chtype  getbkgd(void *w);
int     getch();
int     getnstr(char *a, int b);
int     getstr(char *a);
void*   getwin(void *f);
int     halfdelay(int a);
bool    has_colors();
bool    has_ic();
bool    has_il();
int     hline(chtype b, int c);
void    idcok(void* w, bool b);
int     idlok(void *w, bool b);
void    immedok(void *w, bool b);
chtype  inch();
int     inchnstr(chtype *a, int b);
int     inchstr(chtype *a);
void*   initscr();
int     init_color(short a, short b, short c, short d);
int     init_pair(short a, short b, short c);
int     innstr(char *a, int b);
int     insch(chtype a);
int     insdelln(int a);
int     insertln();
int     insnstr(char *a, int b);
int     insstr(char *a);
int     instr(char *a);
int     intrflush(void *a, bool b);
bool    isendwin();
bool    is_linetouched(void *a, int b);
bool    is_wintouched(void *a);
char*   keyname(int a);
int     keypad(void *a, bool b);
char    killchar();
int     leaveok(void *a, bool b);
char*   longname();
int     meta(void *a, bool b);
int     move(int a, int b);
int     mvaddch(int a, int b, chtype c);
int     mvaddchnstr(int a, int b, chtype *c, int d);
int     mvaddchstr(int a, int b, chtype *c);
int     mvaddnstr(int a, int b, char *c, int d);
int     mvaddstr(int a, int b, char *c);
int     mvchgat(int a, int b, int c, attr_t d, short e, void *f);
int     mvcur(int a, int b, int c, int d);
int     mvdelch(int a, int b);
int     mvderwin(void *a, int b, int c);
int     mvgetch(int a, int b);
int     mvgetnstr(int a, int b, char *c, int d);
int     mvgetstr(int a, int b, char *c);
int     mvhline(int a, int b, chtype c, int d);
chtype  mvinch(int a, int b);
int     mvinchnstr(int a, int b, chtype *c, int d);
int     mvinchstr(int a, int b, chtype *c);
int     mvinnstr(int a, int b, char *c, int d);
int     mvinsch(int a, int b, chtype c);
int     mvinsnstr(int a, int b, char *c, int d);
int     mvinsstr(int a, int b, char *c);
int     mvinstr(int a, int b, char *c);
int     mvprintw(int a, int b, char *c);
int     mvscanw(int a, int b, char *c);
int     mvvline(int a, int b, chtype c, int d);
int     mvwaddch(void *w, int b, int c, chtype d);
int     mvwaddchnstr(void *w, int b, int c, chtype *d, int e);
int     mvwaddchstr(void *w, int b, int c, chtype *d);
int     mvwaddnstr(void *w, int b, int c, char *d, int e);
int     mvwaddstr(void *w, int b, int c, char *d);
int     mvwchgat(void *w, int b, int c, int d, attr_t e, short f, void *g);
int     mvwdelch(void *w, int b, int c);
int     mvwgetch(void *w, int b, int c);
int     mvwgetnstr(void *w, int b, int c, char *d, int e);
int     mvwgetstr(void *w, int b, int c, char *d);
int     mvwhline(void *w, int b, int c, chtype d, int e);
int     mvwin(void *w, int b, int c);
chtype  mvwinch(void *w, int b, int c);
int     mvwinchnstr(void *w, int b, int c, chtype *d, int e);
int     mvwinchstr(void *w, int b, int c, chtype *d);
int     mvwinnstr(void *w, int b, int c, char *s, int d);
int     mvwinsch(void *w, int b, int c, chtype d);
int     mvwinsnstr(void *w, int b, int c, char *s, int d);
int     mvwinsstr(void *w, int b, int c, char *s);
int     mvwinstr(void *w, int b, int c, char *s);
int     mvwprintw(void *w, int b, int c, char *s);
int     mvwscanw(void *w, int b, int c, char *s);
int     mvwvline(void *w, int b, int c, chtype d, int e);
int     napms(int a);
void*   newpad(int a, int b);
void*   newterm(char *a, void *f1, void *f2);
void*   newwin(int a, int b, int c, int d);
int     nl();
int     nocbreak();
int     nodelay(void *w, bool b);
int     noecho();
int     nonl();
void    noqiflush();
int     noraw();
int     notimeout(void *w, bool b);
int     overlay(void *w, void *b);
int     overwrite(void *w, void *b);
int     pair_content(short a, short *b, short *c);
int     PAIR_NUMBER(int a);
int     pechochar(void *w, chtype b);
int     pnoutrefresh(void *w, int b, int c, int d, int e, int f, int g);
int     prefresh(void *w, int b, int c, int d, int e, int f, int g);
int     printw(char *s);
int     putwin(void *w, void *b);
void    qiflush();
int     raw();
int     redrawwin(void *w);
int     refresh();
int     resetty();
int     reset_prog_mode();
int     reset_shell_mode();
int     ripoffline(int a, void *f);
int     savetty();
int     scanw(char *s);
int     scr_dump(char *s);
int     scr_init(char *s);
int     scrl(int a);
int     scroll(void *a);
int     scrollok(void *a, bool b);
int     scr_restore(char *a);
int     scr_set(char *a);
int     setscrreg(int a, int b);
void*   set_term(void *a);
int     slk_attroff(chtype a);
int     slk_attr_off(attr_t a, void *b);
int     slk_attron(chtype a);
int     slk_attr_on(attr_t a, void *b);
int     slk_attrset(chtype a);
attr_t  slk_attr();
int     slk_attr_set(attr_t a, short b, void *c);
int     slk_clear();
int     slk_color(short a);
int     slk_init(int a);
char*   slk_label(int a);
int     slk_noutrefresh();
int     slk_refresh();
int     slk_restore();
int     slk_set(int a, char *b, int c);
int     slk_touch();
int     standout();
int     standend();
int     start_color();
void*   subpad(void *a, int b, int c, int d, int e);
void*   subwin(void *a, int b, int c, int d, int e);
int     syncok(void *a, bool b);
chtype  termattrs();
char*   termname();
void    timeout(int a);
int     touchline(void *w, int b, int c);
int     touchwin(void *w);
int     typeahead(int a);
int     ungetch(int a);
int     untouchwin(void *w);
void    use_env(bool b);
int     vidattr(chtype c);
int     vidputs(chtype c, void *f);
int     vline(chtype c, int b);
int     vwprintw(void *w, char *b);
int     vw_printw(void *w, char *b);
int     vwscanw(void *w, char *b);
int     vw_scanw(void *w, char *b);
int     waddch(void *w, chtype b);
int     waddchnstr(void *w, chtype *b, int c);
int     waddchstr(void *w, chtype *b);
int     waddnstr(void *w,char *b, int b);
int     waddstr(void *w, char *b);
int     wattron(void *w, int b);
int     wattroff(void *w, int b);
int     wattrset(void *w, int b);
int     wattr_get(void *w, attr_t *b, short *c, void *d);
int     wattr_on(void *w, attr_t b, void *c);
int     wattr_off(void *w, attr_t b, void *c);
int     wattr_set(void *w, attr_t b, short c, void *d);
int     wbkgd(void *w, chtype a);
void    wbkgdset(void *w, chtype a);
int     wborder(void *w, chtype a, chtype b, chtype c, chtype d, chtype e, chtype f, chtype g, chtype h);
int     wchgat(void *w, int a, attr_t b, short c, void *p);
int     wclear(void *w);
int     wclrtobot(void *w);
int     wclrtoeol(void *w);
int     wcolor_set(void *w, short b, void *p);
void    wcursyncup(void *w);
int     wdelch(void *w);
int     wdeleteln(void *w);
int     wechochar(void *w, chtype c);
int     werase(void *w);
int     wgetch(void *w);
int     wgetnstr(void *w, char *s, int d);
int     wgetstr(void *w, char *s);
int     whline(void *w, chtype c, int d);
chtype  winch(void *w);
int     winchnstr(void *w, chtype *c, int d);
int     winchstr(void *w, chtype *c);
int     winnstr(void *w, char *s, int c);
int     winsch(void *w, chtype c);
int     winsdelln(void *w, int a);
int     winsertln(void *w);
int     winsnstr(void *w, char *s, int c);
int     winsstr(void *w, char *s);
int     winstr(void *w, char *s);
int     wmove(void *w, int a, int b);
int     wnoutrefresh(void *w);
int     wprintw(void *w, char *s);
int     wredrawln(void *w, int a, int b);
int     wrefresh(void *w);
int     wscanw(void *w, char *s);
int     wscrl(void *w, int a);
int     wsetscrreg(void *w, int a, int b);
int     wstandout(void *w);
int     wstandend(void *w);
void    wsyncdown(void *w);
void    wsyncup(void *w);
void    wtimeout(void *w, int a);
int     wtouchln(void *w, int a, int b, int c);
int     wvline(void *w, chtype a, int b);

int     tigetflag(char *s);
int     tigetnum(char *s);
char    tigetstr(char *s);
int     putp(char *s);
char    tparm(char *s);
char*   tparm_varargs(char *s);

int     getcurx(void *w);
int     getcury(void *w);
int     getbegx(void *w);
int     getbegy(void *w);
int     getmaxx(void *w);
int     getmaxy(void *w);
int     getparx(void *w);
int     getpary(void *w);

int     vsscanf(char *s, char *s1);

typedef long mmask_t;

int     getmouse(void *e);
int     ungetmouse(void *e);
mmask_t mousemask(mmask_t a, mmask_t *b);
bool    wenclose(void *w, int a, int c);
int     mouseinterval(int a);
bool    wmouse_trafo(void *w, int *a, int *b, bool c);
bool    mouse_trafo(int *a, int *b, bool c);

int     mcprint(char *s, int a);
int     has_key(int a);

void    _tracef(char *s);
void    _tracedump(char *s, void *w);
char*   _traceattr(attr_t a);
char*   _traceattr2(int a, chtype c);
char*   _nc_tracebits();
char*   _tracechar(int a);
char*   _tracechtype(chtype c);
char*   _tracechtype2(int a, chtype c);
char*   _tracecchar_t(void *a);
char*   _tracecchar_t2(int a, void *b);
char*   _tracemouse(void *e);
void    trace(u_int a);
