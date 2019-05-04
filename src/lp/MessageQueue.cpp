#include "MessageQueue.h" 
#include "SingleReadResponseMessage.h"
#include "WriteMessage.h"
#include "RangeQueryMessage.h"

using namespace std;
using namespace pdesmas;

MessageQueue::MessageQueue() {
  fMutex = Mutex(NORMAL);
}

MessageQueue::~MessageQueue() {
  // Do nothing
}

bool MessageQueue::IsEmpty() const {
  fMutex.Lock();
  bool isEmpty = (fMessageQueue.size() == 0);
  fMutex.Unlock();
  return isEmpty;
}

void MessageQueue::QueueMessage(AbstractMessage* item) {
  fMutex.Lock();
  fMessageQueue.push_back(item);
  fMutex.Unlock();
}

AbstractMessage* MessageQueue::DequeueMessage() {
  fMutex.Lock();
  AbstractMessage* result = fMessageQueue.front();
  fMessageQueue.pop_front();
  fMutex.Unlock();
  return result;
}

void MessageQueue::RemoveMessages(const LpId& pOriginalAlp, unsigned long pTime) {
  // Lock the queue
  fMutex.Lock();
  // Initialise result
  // If no messages in queue, do nothing
  if (fMessageQueue.size() > 0) {
    // Walk over all messages
    for (deque<AbstractMessage*>::iterator iter = fMessageQueue.begin(); iter != fMessageQueue.end();) {
      switch((*iter)->GetType()) {
        case SINGLEREADMESSAGE : {
          SingleReadMessage* singleReadMessage = static_cast<SingleReadMessage*>(*iter);
          if (singleReadMessage->GetOriginalAgent() == pOriginalAlp && singleReadMessage->GetTimestamp() >= pTime) {
            delete *iter;
            iter = fMessageQueue.erase(iter);
            continue;
          }
        }
          break;
        case WRITEMESSAGE : {
          WriteMessage* writeMessage = static_cast<WriteMessage*>(*iter);
          // Note, the write messages have one plus the LVT (pTime) at a timestep, so no equals here!
          if (writeMessage->GetOriginalAgent() == pOriginalAlp && writeMessage->GetTimestamp() > pTime) {
            writeMessage->ClearValue();
            delete *iter;
            iter = fMessageQueue.erase(iter);
            continue;
          }
        }
          break;
        case RANGEQUERYMESSAGE : {
          RangeQueryMessage* rangeQueryMessage = static_cast<RangeQueryMessage*>(*iter);
          if (rangeQueryMessage->GetOriginalAgent() == pOriginalAlp && rangeQueryMessage->GetTimestamp() >= pTime) {
            delete *iter;
            iter = fMessageQueue.erase(iter);
            continue;
          }
        }
          break;
        default :
          // Skip
          break;
      }
      iter++;
    }
  }
  // Unlock the queue
  fMutex.Unlock();
}
