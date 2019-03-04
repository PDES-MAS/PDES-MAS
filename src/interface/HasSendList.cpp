/*
 * HasSendList.cpp
 *
 *  Created on: 1 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasSendList.h"
#include "SingleReadMessage.h"
#include "WriteMessage.h"
#include "RangeQueryMessage.h"

using namespace pdesmas;

list<SharedStateMessage*> HasSendList::RollbackSendList(unsigned long pTime,
    const LpId& pOriginalAlp) {
  list<SharedStateMessage*> result;
  for (list<SharedStateMessage*>::iterator iter = fSendList.begin(); iter != fSendList.end();) {
    switch ((*iter)->GetType()) {
      case SINGLEREADMESSAGE: {
        SingleReadMessage* singleReadMessage = static_cast<SingleReadMessage*> (*iter);
        // TODO Add copy constructor to all messages
        if (singleReadMessage->GetOriginalAlp() == pOriginalAlp && singleReadMessage->GetTimestamp() >= pTime) {
          SingleReadMessage* copyMessage = new SingleReadMessage;
          *copyMessage = *singleReadMessage;
          result.push_back(copyMessage);
          delete *iter;
          iter = fSendList.erase(iter);
          continue;
        }
      }
        break;
      case WRITEMESSAGE: {
        WriteMessage* writeMessage = static_cast<WriteMessage*> (*iter);
        // Note, again, I don't need to rollback the write message from the previous step, so no equals sign here!
        if (writeMessage->GetOriginalAlp() == pOriginalAlp && writeMessage->GetTimestamp() > pTime) {
          WriteMessage* copyMessage = new WriteMessage;
          *copyMessage = *writeMessage;
          result.push_back(copyMessage);
          delete *iter;
          iter = fSendList.erase(iter);
          continue;
        }
      }
        break;
      case RANGEQUERYMESSAGE: {
        RangeQueryMessage* rangeQueryMessage = static_cast<RangeQueryMessage*> (*iter);
        if (rangeQueryMessage->GetOriginalAlp() == pOriginalAlp && rangeQueryMessage->GetTimestamp() >= pTime) {
          RangeQueryMessage* copyMessage = new RangeQueryMessage;
          *copyMessage = *rangeQueryMessage;
          result.push_back(copyMessage);
          delete *iter;
          iter = fSendList.erase(iter);
          continue;
        }
      }
        break;
      default:
        // Skip
        break;
    }
    iter++;
  }
  return result;
}

void HasSendList::ClearSendList(unsigned long pTime) {
  for (list<SharedStateMessage*>::iterator iter = fSendList.begin(); iter
      != fSendList.end();) {
    if ((*iter)->GetTimestamp() < pTime) {
      // I don't need to free the value memory for WriteMessages here because that's already done
      // in the SendThread!
      delete *iter;
      iter = fSendList.erase(iter);
    } else iter++;
  }
}

void HasSendList::AddToSendList(const SingleReadMessage* pSingleReadMessage) {
  SingleReadMessage* copyMessage = new SingleReadMessage;
  *copyMessage = *pSingleReadMessage;
  fSendList.push_back(copyMessage);
}

void HasSendList::AddToSendList(const WriteMessage* pWriteMessage) {
  WriteMessage* copyMessage = new WriteMessage;
  *copyMessage = *pWriteMessage;
  fSendList.push_back(copyMessage);
}

void HasSendList::AddToSendList(const RangeQueryMessage* pRangeQueryMessage) {
  RangeQueryMessage* copyMessage = new RangeQueryMessage;
  *copyMessage = *pRangeQueryMessage;
  fSendList.push_back(copyMessage);
}
