
/* /usr/include/arpa/inet.h */

struct in_addr {
	in_addr_t s_addr;
};

in_addr_t 	inet_addr(char *__cp);
in_addr_t 	inet_lnaof(struct in_addr __in);
struct in_addr 	inet_makeaddr(in_addr_t __net, in_addr_t __host);
in_addr_t 	inet_netof(struct in_addr __in);
in_addr_t 	inet_network(char *__cp);
char*		inet_ntoa(struct in_addr __in);
int 		inet_pton(int __af, char *__cp, void __buf);
char*		inet_ntop(int __af, void *__cp, char *__buf, socklen_t __len);
int 		inet_aton(char *__cp, struct in_addr *__inp);
char*		inet_neta(in_addr_t __net, char *__buf, size_t __len);
char*		inet_net_ntop(int __af, void *__cp, int __bits, char *__buf, size_t __len);
