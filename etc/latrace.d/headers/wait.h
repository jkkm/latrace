
/* /usr/include/sys/wait.h */


typedef u_int __WAIT_STATUS;
typedef u_int idtype_t;
typedef u_int __id_t;


__pid_t wait(__WAIT_STATUS stat_loc);
__pid_t waitpid(__pid_t pid, int *stat_loc, int options);


int waitid(idtype_t idtype, __id_t id, void *infop, int options);


__pid_t wait3(__WAIT_STATUS stat_loc, int options, void *usage);
__pid_t wait4(__pid_t pid, __WAIT_STATUS stat_loc, int options, void *usage);
