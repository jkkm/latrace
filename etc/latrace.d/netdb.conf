
/* /usr/include/netdb.h */


int*    __h_errno_location();
void    herror(char *str);
char*   hstrerror(int err_num);


struct hostent {
        char* h_name;
        void* h_aliases;
        int   h_addrtype;
        int   h_length;
        void* h_addr_list;
};


void sethostent(int stay_open);
void endhostent();
struct hostent* gethostent();
struct hostent* gethostbyaddr(void *addr, socklen_t len, int type);
struct hostent* gethostbyname(char *name);
struct hostent* gethostbyname2(char *name, int af);


int gethostent_r(struct hostent *result_buf, char *buf, size_t buflen, void *result, int *h_errnop);
int gethostbyaddr_r(void *addr, socklen_t len, int type, struct hostent *result_buf, char *buf, size_t buflen, void *result, int *h_errnop);
int gethostbyname_r(char *name, struct hostent *result_buf, char *buf, size_t buflen, void *result, int *h_errnop);
int gethostbyname2_r(char *name, int af, struct hostent *result_buf, char *buf, size_t buflen, void *result, int *h_errnop);


void  setnetent(int stay_open);
void  endnetent();
void* getnetent();
void* getnetbyaddr(uint32_t net, int type);
void* getnetbyname(char *name);
int   getnetent_r(void *result_buf, char *buf, size_t buflen,void *result, int *h_errnop);
int   getnetbyaddr_r(uint32_t net, int type, void *result_buf, char *buf, size_t buflen, void *result, int *h_errnop);
int   getnetbyname_r(char *name, void *result_buf, char *buf, size_t buflen, void *result, int *h_errnop);


struct servent {
        char *s_name;
        void *s_aliases;
        int   s_port;
        char *s_proto;
};


void setservent(int stay_open);
void endservent();
struct servent* getservent();
struct servent* getservbyname(char *name, char *proto);
struct servent* getservbyport(int port, char *proto);
int getservent_r(struct servent *result_buf, char *buf, size_t buflen, void *result);
int getservbyname_r(char *name, char *proto, struct servent *result_buf, char *buf, size_t buflen, void *result);
int getservbyport_r(int port, char *proto, struct servent *result_buf, char *buf, size_t buflen, void *result);


struct protoent {
        char *p_name;
        char *p_aliases;
        int   p_proto;
};


void setprotoent(int stay_open);
void endprotoent();
struct protoent* getprotoent();
struct protoent* getprotobyname(char *name);
struct protoent* getprotobynumber(int proto);
int getprotoent_r(struct protoent* result_buf, char *buf, size_t buflen, void *result);
int getprotobyname_r(char *name, struct protoent *result_buf, char *buf, size_t buflen, void *result);
int getprotobynumber_r(int proto, struct protoent* result_buf, char *buf, size_t buflen, void *result);


int setnetgrent(char *netgroup);
void endnetgrent();
int getnetgrent(void *hostp, void *userp, void *domainp);
int innetgr(char *netgroup, char *host, char *user, char *domain);
int getnetgrent_r(void *hostp, void *userp, void *domainp, char *buffer, size_t buflen);


int rcmd(void *ahost, u_short rport, char *locuser, char *remuser, char *cmd, int *fd2p);
int rcmd_af(void *ahost, u_short rport, char *locuser, char *remuser, char *cmd, int *fd2p, u_int af);


int rexec(void *ahost, int rport, char *name, char *pass, char *cmd, int *fd2p);
int rexec_af(void *ahost, int rport, char *name, char *pass, char *cmd, int *fd2p, u_int af);


int ruserok(char *rhost, int suser, char *remuser, char *locuser);
int ruserok_af(char *rhost, int suser, char *remuser, char *locuser, u_int af);


int rresvport(int *alport);
int rresvport_af(int *alport, u_int af);


int  getaddrinfo(char *name, char *service, void *req, void *pai);
void freeaddrinfo(void *ai);


char* gai_strerror(int ecode);
int   getnameinfo(void *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, u_int flags);
int   getaddrinfo_a(int mode, void *list, int ent, void *sig);
int   gai_suspend(void *list, int ent, void *timeout);
int   gai_error(void *req);
int   gai_cancel(void *gaicbp);
