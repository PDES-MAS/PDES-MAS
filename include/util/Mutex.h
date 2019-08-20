/*
 * Mutex.h
 *
 *  Created on: 9 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <pthread.h>
#include <mutex>
#include <thread>
namespace pdesmas {
  enum mutex_t {NORMAL, RECURSIVE, ERRORCHECK};
  class Mutex {
    private:
      pthread_mutex_t fMutex;
      pthread_mutexattr_t fMutexAttribute;
      pthread_cond_t fMutexCondition;
      void Initialise(mutex_t);
    public:
      Mutex();
      Mutex(mutex_t);
      virtual ~Mutex();

      int Lock();
      int Unlock();
      int Wait();
      int Wait(timespec);
      void Signal();
  };
}

#endif /* MUTEX_H_ */
