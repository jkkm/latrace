
/* /usr/include/pthread.h */


typedef void   __clockid_t;
typedef u_long pthread_t;
typedef void   pthread_attr_t;
typedef void   sched_param;
typedef void   cpu_set_t;
typedef void   pthread_once_t;
typedef void   pthread_mutex_t;
typedef void   pthread_mutexattr_t;
typedef void   pthread_rwlock_t;
typedef void   pthread_rwlockattr_t;
typedef void   pthread_cond_t;
typedef void   pthread_condattr_t;
typedef void   pthread_barrier_t;
typedef void   pthread_spinlock_t;
typedef void   pthread_barrierattr_t;
typedef u_int  pthread_key_t;


int  pthread_create(pthread_t *newthread, pthread_attr_t *attr, void *start_routine, void *__arg);
void pthread_exit(void *retval);
int  pthread_join(pthread_t th, void *thread_return);
int  pthread_tryjoin_np(pthread_t th, void *thread_return);
int  pthread_timedjoin_np(pthread_t th, void *thread_return, timespec *abstime);
int  pthread_detach(pthread_t th);
int  pthread_equal(pthread_t thread1, pthread_t thread2);


pthread_t pthread_self();


int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);
int pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getguardsize(pthread_attr_t *attr, size_t *guardsize);
int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
int pthread_attr_getschedparam(pthread_attr_t *attr, sched_param *param);
int pthread_attr_setschedparam(pthread_attr_t *attr, sched_param *param);
int pthread_attr_getschedpolicy(pthread_attr_t *attr, int *policy);
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getinheritsched(pthread_attr_t *attr, int *inherit);
int pthread_attr_setinheritsched(pthread_attr_t *attr, int inherit);
int pthread_attr_getscope(pthread_attr_t *attr, int *scope);
int pthread_attr_setscope(pthread_attr_t *attr, int scope);
int pthread_attr_getstackaddr(pthread_attr_t *attr, void *stackaddr);
int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int pthread_attr_getstack(pthread_attr_t *attr, void *stackaddr, size_t *stacksize);
int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
int pthread_attr_setaffinity_np(pthread_attr_t *attr, size_t cpusetsize, cpu_set_t *cpuset);
int pthread_attr_getaffinity_np(pthread_attr_t *attr, size_t cpusetsize, cpu_set_t *cpuset);
int pthread_getattr_np(pthread_t th, pthread_attr_t *attr);


int pthread_setschedparam(pthread_t target_thread, int policy, sched_param *param);
int pthread_getschedparam(pthread_t target_thread, int *policy, sched_param *param);
int pthread_setschedprio(pthread_t target_thread, int prio);
int pthread_getconcurrency();
int pthread_setconcurrency(int level);
int pthread_yield();


int pthread_setaffinity_np(pthread_t th, size_t cpusetsize, cpu_set_t *cpuset);
int pthread_getaffinity_np(pthread_t th, size_t cpusetsize, cpu_set_t *cpuset);
int pthread_once(pthread_once_t *once_control, void *init_routine);
int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
int pthread_cancel(pthread_t th);
void pthread_testcancel();


int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t *mutexattr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_timedlock(pthread_mutex_t *mutex, timespec *abstime);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_getprioceiling(pthread_mutex_t *mutex, int *prioceiling);
int pthread_mutex_setprioceiling(pthread_mutex_t *mutex, int prioceiling, int *old_ceiling);
int pthread_mutex_consistent_np(pthread_mutex_t *mutex);


int pthread_mutexattr_init(pthread_mutexattr_t *attr);
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int pthread_mutexattr_getpshared(pthread_mutexattr_t *attr, int *pshared);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared);
int pthread_mutexattr_gettype(pthread_mutexattr_t *attr, int *kind);
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);
int pthread_mutexattr_getprotocol(pthread_mutexattr_t *attr, int *protocol);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
int pthread_mutexattr_getprioceiling(pthread_mutexattr_t *attr, int *prioceiling);
int pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr, int prioceiling);
int pthread_mutexattr_getrobust_np(pthread_mutexattr_t *attr, int *robustness);
int pthread_mutexattr_setrobust_np(pthread_mutexattr_t *attr, int robustness);


int pthread_rwlock_init(pthread_rwlock_t *rwlock, pthread_rwlockattr_t *attr);
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *rwlock, timespec *abstime);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock, timespec *abstime);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);


int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
int pthread_rwlockattr_getpshared(pthread_rwlockattr_t *attr, int *pshared);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared);
int pthread_rwlockattr_getkind_np(pthread_rwlockattr_t *attr, int *pref);
int pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr, int pref);


int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, timespec *abstime);


int pthread_condattr_init(pthread_condattr_t *attr);
int pthread_condattr_destroy(pthread_condattr_t *attr);
int pthread_condattr_getpshared(pthread_condattr_t *attr, int *pshared);
int pthread_condattr_setpshared(pthread_condattr_t *attr, int pshared);
int pthread_condattr_getclock(pthread_condattr_t *attr, __clockid_t *clock_id);
int pthread_condattr_setclock(pthread_condattr_t *attr, __clockid_t clock_id);


int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int pthread_spin_destroy(pthread_spinlock_t *lock);
int pthread_spin_lock(pthread_spinlock_t *lock);
int pthread_spin_trylock(pthread_spinlock_t *lock);
int pthread_spin_unlock(pthread_spinlock_t *lock);


int pthread_barrier_init(pthread_barrier_t *barrier, pthread_barrierattr_t *attr, u_int count);
int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_wait(pthread_barrier_t *barrier);


int pthread_barrierattr_init(pthread_barrierattr_t *attr);
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int pthread_barrierattr_getpshared(pthread_barrierattr_t *attr, int *pshared);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);


int   pthread_key_create(pthread_key_t *key, void *destr_function);
int   pthread_key_delete(pthread_key_t key);
void* pthread_getspecific(pthread_key_t key);
int   pthread_setspecific(pthread_key_t key, void *pointer);


int pthread_getcpuclockid(pthread_t thread_id, __clockid_t *clock_id);
int pthread_atfork(void *prepare, void *parent, void *child);
