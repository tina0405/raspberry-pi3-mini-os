#include "threadtype.h"
#include <stddef.h>
#include "sched.h"
/* Lock MUTEX, block if we can't get it.  */
int __pthread_mutex_lock (struct thread_mutex *mutex)
{
  int err;
  int drain;
  struct  pcb_struct *self;
  const struct __pthread_mutexattr *attr = mutex->__attr;

  if (attr == &__pthread_errorcheck_mutexattr)
    attr = &__pthread_errorcheck_mutexattr;
  if (attr == &__pthread_recursive_mutexattr)
    attr = &__pthread_recursive_mutexattr;
  //__pthread_spin_lock (&mutex->__lock);
  
/*
   if (__pthread_spin_trylock (&mutex->__held) == 0)
    //Successfully acquired the lock.
    {

      if (attr != NULL)
	switch (attr->__mutex_type)
	{
	  case __PTHREAD_MUTEX_NORMAL:
	    break;

	  case __PTHREAD_MUTEX_RECURSIVE:
	    mutex->__locks = 1;
	  case __PTHREAD_MUTEX_ERRORCHECK:
	    mutex->__owner = thread_self ();
	    break;

	  default:
	    error();
	}

      //__pthread_spin_unlock (&mutex->__lock);
      return 0;
    }
*/
  /* The lock is busy.  */

  self = thread_self ();
  //assert (self);

  if (attr == NULL || attr->__mutex_type == __PTHREAD_MUTEX_NORMAL){}
  else
  {
      switch (attr->__mutex_type)
      {
	case __PTHREAD_MUTEX_ERRORCHECK:
	  if (mutex->__owner == self)
	    {
	      //__pthread_spin_unlock (&mutex->__lock);
	      error();
	    }
	  break;

	case __PTHREAD_MUTEX_RECURSIVE:
	  if (mutex->__owner == self)
	    {
	      mutex->__locks++;
	      //__pthread_spin_unlock (&mutex->__lock);
	      return 0;
	    }
	  break;

	default:
	  error();
	}
    }


    //assert (mutex->__owner);



  /* Add ourselves to the queue.  */
  //__pthread_enqueue (&mutex->__queue, self);
  //__pthread_spin_unlock (&mutex->__lock);
//
  /* Block the thread.  */
  err = 0;
  thread_block (self);
   

  //__pthread_spin_lock (&mutex->__lock);
  if (self->prevp == NULL)
    /* Another thread removed us from the queue, which means a wakeup message
       has been sent.  It was either consumed while we were blocking, or
       queued after we timed out and before we acquired the mutex lock, in
       which case the message queue must be drained.  */
    drain = err ? 1 : 0;
  else
    {
      /* We're still in the queue.  Noone attempted to wake us up, i.e. we
         timed out.  */
      //__pthread_dequeue (self);
      drain = 0;
    }
  //__pthread_spin_unlock (&mutex->__lock);

  if (drain)
    //__pthread_block (self);

  if (err)
  {
      error();/*timeout*/
      return err;
   }

      //assert (mutex->__owner == self);


  if (attr != NULL)
      switch (attr->__mutex_type)
      {
      	case __PTHREAD_MUTEX_NORMAL:
		break;

      	case __PTHREAD_MUTEX_RECURSIVE:
		//assert (mutex->__locks == 0);
		mutex->__locks = 1;
     	 case __PTHREAD_MUTEX_ERRORCHECK:
		mutex->__owner = self;
		break;

      	default:
		error();
      }

  return 0;
}

