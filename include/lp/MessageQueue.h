#ifndef _MESSAGEQUEUE_H_
#define _MESSAGEQUEUE_H_

#include <deque>
#include "AbstractMessage.h"
#include "Mutex.h"
#include "LpId.h"

using namespace std;

namespace pdesmas {
  class MessageQueue {
    private:
      deque<AbstractMessage*> fMessageQueue;
      mutable Mutex fMutex;
    public:
      MessageQueue();
      ~MessageQueue();

      bool IsEmpty() const;
      void QueueMessage(AbstractMessage*);
      AbstractMessage* DequeueMessage();
      void RemoveMessages(const LpId&, unsigned long);
  };
}
#endif
