/*
 * Thread.cpp
 *
 *  Created on: 10 Mar 2010
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#include <iostream>
#include "Thread.h"

using namespace pdesmas;

void *thread_routine(void *pArgument) {
  Thread *thread = reinterpret_cast<Thread *> (pArgument);
  thread->MyThread(pArgument);
  return 0;
}

Thread::Thread() :
    fThreadID(0) {
}

Thread::~Thread() {
  if (fThreadID != 0) Stop();
}

bool Thread::Start(void *pArgument) {
  int status;
  this->Sleep(100);
  status = pthread_create(&fThreadID, 0, thread_routine, pArgument);
  return (status == 0);
}

bool Thread::Detach() {
  int status;
  status = pthread_detach(fThreadID);
  return (status == 0);
}

bool Thread::Stop() {

  pthread_cancel(fThreadID);
  return true;
}

bool Thread::Join() {
  char *receiveValue;
  pthread_join(fThreadID, (void **) &receiveValue);
  return true;
}

void Thread::Sleep(unsigned long pMiliseconds) {
  struct timespec rqtp, rmtp;
  rqtp.tv_nsec = (pMiliseconds % 1000) * 1000000;
  rqtp.tv_sec = pMiliseconds / 1000;
  nanosleep(&rqtp, &rmtp);
}

int Thread::GetThreadId() const {
  return (int) fThreadID;
}

void Thread::Wait() {
  fSemaphore.Wait();
}

void Thread::Signal() {
  fSemaphore.Signal();
}
