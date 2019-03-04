/*
 * HasResponseMessage.cpp
 *
 *  Created on: 12 Apr 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasResponseMessage.h"

using namespace pdesmas;

void HasResponseMessage::SetResponseMessage(const RollbackMessage* pRollbackMessage) {
  fResponseMessageMutex.Lock();
  RollbackMessage* copy = new RollbackMessage;
  *copy = *pRollbackMessage;
  fResponseMessage = copy;
  fResponseMessageMutex.Unlock();
}

void HasResponseMessage::SetResponseMessage(const SingleReadResponseMessage* pSingleReadMessageResponse) {
  fResponseMessageMutex.Lock();
  SingleReadResponseMessage* copy = new SingleReadResponseMessage;
  *copy = *pSingleReadMessageResponse;
  fResponseMessage = copy;
  fResponseMessageMutex.Unlock();
}

void HasResponseMessage::SetResponseMessage(const WriteResponseMessage* pWriteMessageResponse) {
  fResponseMessageMutex.Lock();
  WriteResponseMessage* copy = new WriteResponseMessage;
  *copy = *pWriteMessageResponse;
  fResponseMessage = copy;
  fResponseMessageMutex.Unlock();
}

void HasResponseMessage::SetResponseMessage(const RangeQueryMessage* pRangeQueryMessage) {
  fResponseMessageMutex.Lock();
  RangeQueryMessage* copy = new RangeQueryMessage;
  *copy = *pRangeQueryMessage;
  fResponseMessage = copy;
  fResponseMessageMutex.Unlock();
}

AbstractMessage* HasResponseMessage::GetResponseMessage() const {
  fResponseMessageMutex.Lock();
  AbstractMessage* result = fResponseMessage;
  fResponseMessageMutex.Unlock();
  return result;
}
