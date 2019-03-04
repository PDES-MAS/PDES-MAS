/*
 * Mutex.cpp
 *
 *  Created on: 9 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#include <errno.h>
#include "Mutex.h"
#include "Log.h"

using namespace pdesmas;

Mutex::Mutex() {
  Initialise(RECURSIVE);
}

Mutex::Mutex(mutex_t pMutexType) {
  Initialise(pMutexType);
}

Mutex::~Mutex() {
  // Nothing
}

int Mutex::Lock() {
  int errorCode = pthread_mutex_lock(&fMutex);
  if (errorCode != 0) {
    LOG(logERROR)
    << "Mutex::Mutex# Failed to lock Mutex, errorcode: " << errorCode;
  }
  return errorCode;
}

int Mutex::Unlock() {
  int errorCode = pthread_mutex_unlock(&fMutex);
  if (errorCode != 0) {
    LOG(logERROR)
    << "Mutex::Mutex# Failed to unlock Mutex, errorcode: " << errorCode;
    switch (errorCode) {
      case EINVAL:
        LOG(logERROR)
        << "Mutex::Mutex# Mutex not initialised";
        break;
      case EPERM:
        LOG(logERROR)
        << "Mutex::Mutex# Mutex already unlocked";
        break;
      default:
        LOG(logERROR)
        << "Mutex::Mutex# Undefined error";
    }
  }
  return errorCode;
}

int Mutex::Wait() {
  return pthread_cond_wait(&fMutexCondition, &fMutex);
}

int Mutex::Wait(timespec pTime) {
  return pthread_cond_timedwait(&fMutexCondition, &fMutex, &pTime);
}

void Mutex::Signal() {
  pthread_cond_signal(&fMutexCondition);
}

void Mutex::Initialise(mutex_t pMutexType) {
  pthread_mutexattr_init(&fMutexAttribute);
  switch (pMutexType) {
    case NORMAL:
      pthread_mutexattr_settype(&fMutexAttribute, PTHREAD_MUTEX_NORMAL);
      break;
    case RECURSIVE:
      pthread_mutexattr_settype(&fMutexAttribute, PTHREAD_MUTEX_RECURSIVE);
      break;
    case ERRORCHECK:
      pthread_mutexattr_settype(&fMutexAttribute, PTHREAD_MUTEX_ERRORCHECK);
      break;
  }
  int errorCode = pthread_mutex_init(&fMutex, &fMutexAttribute);
  pthread_cond_init(&fMutexCondition, NULL);
  if (errorCode != 0) {
    LOG(logERROR)
    << "Mutex::Initialise# Failed to initialise Mutex, errorcode: " << errorCode;
    switch (errorCode) {
      case EBUSY:
        LOG(logERROR)
            << "Mutex::Initialise# Mutex cannot be reinitialised or modified because it still exists";
        break;
      case EINVAL:
        LOG(logERROR)
        << "Mutex::Initialise# Invalid attribute value, Mutex has not been modified";
        break;
      case EFAULT:
        LOG(logERROR)
        << "Mutex::Initialise# MP address for Mutex is invalid";
        break;
      default:
        LOG(logERROR)
        << "Mutex::Initialise# Undefined error";
        break;
    }
  }
}
