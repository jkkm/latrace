
/* /usr/include/signal.h */

enum SIGNALS {
	SIGHUP    = 1,
	SIGINT    = 2,
	SIGQUIT   = 3,
	SIGILL    = 4,
	SIGTRAP   = 5,
	SIGABRT   = 6,
	SIGIOT    = 6,
	SIGBUS    = 7,
	SIGFPE    = 8,
	SIGKILL   = 9,
	SIGUSR1   = 10,
	SIGSEGV   = 11,
	SIGUSR2   = 12,
	SIGPIPE   = 13,
	SIGALRM   = 14,
	SIGTERM   = 15,
	SIGSTKFLT = 16,
	SIGCHLD   = 17,
	SIGCONT   = 18,
	SIGSTOP   = 19,
	SIGTSTP   = 20,
	SIGTTIN   = 21,
	SIGTTOU   = 22,
	SIGURG    = 23,
	SIGXCPU   = 24,
	SIGXFSZ   = 25,
	SIGVTALRM = 26,
	SIGPROF   = 27,
	SIGWINCH  = 28,
	SIGIO     = 29,
	SIGPWR    = 30,
	SIGSYS    = 31,
	SIGUNUSED = 31,
	SIGRTMIN  = 32,
	SIGRTMAX  = 32
};

enum SIGNAL_HANDLER {
	SIG_DFL = 0,
	SIG_IGN = 1,
	SIG_ERR = -1
};


void*   __sysv_signal(int sig = SIGNALS, void *handler = SIGNAL_HANDLER);
void*   sysv_signal(int sig = SIGNALS, void *handler = SIGNAL_HANDLER);
void*   signal(int sig = SIGNALS, void *handler = SIGNAL_HANDLER);
void*   bsd_signal(int sig = SIGNALS, void *handler = SIGNAL_HANDLER);


int     kill(__pid_t pid, int sig = SIGNALS);
int     killpg(__pid_t pgrp, int sig = SIGNALS);
int     raise(int sig = SIGNALS);


void*   ssignal(int sig = SIGNALS, void *handler = SIGNAL_HANDLER);
int     gsignal(int sig = SIGNALS);
void    psignal(int sig = SIGNALS, char *s);


int     __sigpause(int sig_or_mask, int is_sig);
int     sigpause(int mask);
int     sigblock(int mask);


int     sigsetmask(int mask);
int     siggetmask();
int     sigemptyset(void *set);
int     sigfillset(void *set);
int     sigaddset(void *set, int signo = SIGNALS);
int     sigdelset(void *set, int signo = SIGNALS);
int     sigismember(void *set, int signo = SIGNALS);
int     sigisemptyset(void *set);
int     sigandset(void *set, void *left, void *right);
int     sigorset(void *set, void *left, void *right);
int     sigprocmask(int how, void *set, void *oset);
int     sigsuspend(void *set);
int     sigaction(int sig = SIGNALS, void *act, void *oact);
int     sigpending(void *set);
int     sigwait(void *set, int *sig);
int     sigwaitinfo(void *set, void *info);
int     sigtimedwait(void *set, void *info, void *__timeout);


int     sigqueue(__pid_t pid, int sig = SIGNALS, u_int val);
int     sigvec(int sig = SIGNALS, void *vec, void *ovec);
int     sigreturn(void *scp);
int     siginterrupt(int sig = SIGNALS, int interrupt);
int     sigstack(void *ss, void *oss);
int     sigaltstack(void *ss, void *oss);
int     sighold(int sig = SIGNALS);
int     sigrelse(int sig = SIGNALS);
int     sigignore(int sig = SIGNALS);
void*   sigset(int sig = SIGNALS, void *disp);
int     __libc_current_sigrtmin();
int     __libc_current_sigrtmax();
