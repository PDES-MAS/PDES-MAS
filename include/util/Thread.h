/*
 * Thread.h
 *
 *  Created on: 10 Mar 2010
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef _THREAD_H_
#define _THREAD_H_

#include <iostream>
#include "Semaphore.h"
#include <thread>

extern "C" void* thread_routine(void*);

namespace pdesmas {
  class Thread {
    private:
      pthread_t fThreadID;
      Semaphore fSemaphore;
    public:
      Thread();
      virtual ~Thread();

      int GetThreadId() const;
      bool Start(void*);

    bool Detach();
      bool Stop();
      bool Join();
      void Sleep(unsigned long);
      void Wait();
      void Signal();
      virtual void* MyThread(void*)=0;
  };
}

#endif //_THREAD_H_

