
/* /usr/include/libintl.h */

char* gettext(char *msgid);
char* dgettext(char *domainname, char *msgid);
char* __dgettext(char *domainname, char *msgid);
char* dcgettext(char *domainname, char *msgid, int category);
char* __dcgettext(char *domainname, char *msgid, int category);
char* ngettext(char *msgid1, char *msgid2, u_long n);
char* dngettext(char *domainname, char *msgid1, char *msgid2, u_long n);
char* dcngettext(char *domainname, char *msgid1, char *msgid2, u_long n, int category);
char* textdomain(char *domainname);
char* bindtextdomain(char *domainname, char *dirname);
char* bind_textdomain_codeset(char *domainname, char *codeset);
