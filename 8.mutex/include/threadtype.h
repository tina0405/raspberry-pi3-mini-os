# define PTHREAD_MUTEX_DEFAULT __PTHREAD_MUTEX_NORMAL

enum __pthread_process_shared
{
  __PTHREAD_PROCESS_PRIVATE = 0,
  __PTHREAD_PROCESS_SHARED
};

enum __pthread_mutex_type
{
  __PTHREAD_MUTEX_NORMAL,
  __PTHREAD_MUTEX_ERRORCHECK,
  __PTHREAD_MUTEX_RECURSIVE
};

enum __pthread_mutex_protocol
{
  __PTHREAD_PRIO_NONE = 0,
  __PTHREAD_PRIO_INHERIT,
  __PTHREAD_PRIO_PROTECT
};
/* This structure describes the attributes of a POSIX mutex
   attribute.  */
/*
struct __pthread_mutexattr
{
  int __prioceiling;
  enum __pthread_mutex_protocol __protocol;
  enum __pthread_process_shared __pshared;
  enum __pthread_mutex_type __mutex_type;
};


const struct __pthread_mutexattr __pthread_default_mutexattr = {
  __prioceiling: 0,
  __protocol: __PTHREAD_PRIO_NONE,
  __pshared: __PTHREAD_PROCESS_PRIVATE,
  __mutex_type: PTHREAD_MUTEX_DEFAULT
};

const struct __pthread_mutexattr __pthread_errorcheck_mutexattr = {
  __prioceiling: 0,
  __protocol: __PTHREAD_PRIO_NONE,
  __pshared: __PTHREAD_PROCESS_PRIVATE,
  __mutex_type: __PTHREAD_MUTEX_ERRORCHECK
};

const struct __pthread_mutexattr __pthread_recursive_mutexattr = {
  __prioceiling: 0,
  __protocol: __PTHREAD_PRIO_NONE,
  __pshared: __PTHREAD_PROCESS_PRIVATE,
  __mutex_type: __PTHREAD_MUTEX_RECURSIVE
};
*/

/*
struct __pthread_condattr
{
  enum __pthread_process_shared __pshared;
  __clockid_t __clock;
};

*/
