#ifndef THREADING_H_
#define THREADING_H_

#include "spinlock.h"

// Note that USE_SPINLOCK trumps BOWTIE_PTHREADS

#ifdef USE_SPINLOCK
#  include "spinlock.h"
#  define MUTEX_T SpinLock
#  define MUTEX_INIT(l)
#  define MUTEX_LOCK(l) l.Enter()
#  define MUTEX_UNLOCK(l) l.Leave()
#else
#  ifdef BOWTIE_PTHREADS
#    include <pthread.h>
#    define MUTEX_T pthread_mutex_t
#    define MUTEX_INIT(l) pthread_mutex_init(&l, NULL)
#    define MUTEX_LOCK(l) pthread_mutex_lock(&l)
#    define MUTEX_UNLOCK(l) pthread_mutex_unlock(&l)

static inline void join(pthread_t th) {
	int ret;
	if((ret = pthread_detach(th)) != 0) {
		cerr << "Error: pthread_detach returned non-zero status: " << ret << endl;
		throw 1;
	}
	int *tmp;
	if((ret = pthread_join(th, (void**)&tmp)) != 0) {
		cerr << "Error: pthread_join returned non-zero status: " << ret << endl;
		throw 1;
	}
}

#  else
#    define MUTEX_T int
#    define MUTEX_INIT(l) l = 0
#    define MUTEX_LOCK(l) l = 1
#    define MUTEX_UNLOCK(l) l = 0
#  endif /* BOWTIE_PTHREADS */
#endif /* USE_SPINLOCK */

#endif
