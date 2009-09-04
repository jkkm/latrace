
/* /usr/include/sys/resource.h */

/* Kinds of resource limit.  */
enum rlimit_resource
{
  /* Per-process CPU limit, in seconds.  */
  RLIMIT_CPU = 0,
  /* Largest file that can be created, in bytes.  */
  RLIMIT_FSIZE = 1,
  /* Maximum size of data segment, in bytes.  */
  RLIMIT_DATA = 2,
  /* Maximum size of stack segment, in bytes.  */
  RLIMIT_STACK = 3,
  /* Largest core file that can be created, in bytes.  */
  RLIMIT_CORE = 4,
  /* Largest resident set size, in bytes.
     This affects swapping; processes that are exceeding their
     resident set size will be more likely to have physical memory
     taken from them.  */
  RLIMIT_RSS = 5,
  /* Number of open files.  */
  RLIMIT_NOFILE = 7,
  RLIMIT_OFILE = RLIMIT_NOFILE, /* BSD name for same.  */
  /* Address space limit.  */
  RLIMIT_AS = 9,
  /* Number of processes.  */
  RLIMIT_NPROC = 6,
  /* Locked-in-memory address space.  */
  RLIMIT_MEMLOCK = 8,
  /* Maximum number of file locks.  */
  RLIMIT_LOCKS = 10,
  /* Maximum number of pending signals.  */
  RLIMIT_SIGPENDING = 11,
  /* Maximum bytes in POSIX message queues.  */
  RLIMIT_MSGQUEUE = 12,
  /* Maximum nice priority allowed to raise to.
     Nice levels 19 .. -20 correspond to 0 .. 39
     values of this resource limit.  */
  RLIMIT_NICE = 13,
  /* Maximum realtime priority allowed for non-priviledged
     processes.  */
  LIMIT_RTPRIO = 14,
  RLIMIT_NLIMITS = 15
};

/* Whose usage statistics do you want?  */
enum rusage_who
{
  /* The calling process.  */
  RUSAGE_SELF = 0,
  /* All of its terminated child processes.  */
  RUSAGE_CHILDREN = -1,
  /* The calling thread.  */
  RUSAGE_THREAD = 1
};

enum priority_which
{
  PRIO_PROCESS = 0,		/* WHO is a process ID.  */
  PRIO_PGRP = 1,		/* WHO is a process group ID.  */
  PRIO_USER = 2			/* WHO is a user ID.  */
};

extern int getrlimit(int resource = rlimit_resource, void *rlimits);
extern int getrlimit64(int resource = rlimit_resource, void *rlimits);
extern int setrlimit(int resource = rlimit_resource, void *rlimits);
extern int setrlimit64(int resource = rlimit_resource, void *rlimits);

extern int getrusage(int who = rusage_who, void *usage);

extern int getpriority(int which = priority_which, int who);
extern int setpriority (int which = priority_which, int who, int prio);
