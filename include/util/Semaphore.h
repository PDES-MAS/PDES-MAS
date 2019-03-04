/*
 * Log.h
 *
 *  Created on: 11 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#include "Mutex.h"

namespace pdesmas {
  class Semaphore {
    private:
      unsigned int fCount;
      unsigned int fWaiting;
      Mutex fMutex;
    public:
      Semaphore();

      void Init(unsigned int);
      void Wait();
      void Signal();
  };
}
#endif
