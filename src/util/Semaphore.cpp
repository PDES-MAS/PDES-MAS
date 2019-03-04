/*
 * Log.h
 *
 *  Created on: 11 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#include "Semaphore.h"
using namespace pdesmas;

Semaphore::Semaphore() {
  Init(0);
}

void Semaphore::Init(unsigned int pInitCount) {
  fMutex = Mutex(ERRORCHECK);
  fCount = pInitCount;
  fWaiting = 0;
}

void Semaphore::Wait() {
  fMutex.Lock();
  fWaiting++;
  while (fCount == 0)
    fMutex.Wait();
  fWaiting--;
  fCount--;
  fMutex.Unlock();
}

void Semaphore::Signal() {
  fMutex.Lock();
  if (fWaiting > 0) fMutex.Signal();
  fCount++;
  fMutex.Unlock();
}
