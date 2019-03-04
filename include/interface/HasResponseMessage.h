/*
 * HasResponseMessage.h
 *
 *  Created on: 12 Apr 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASRESPONSEMESSAGE_H_
#define HASRESPONSEMESSAGE_H_

#include "AbstractMessage.h"
#include "RollbackMessage.h"
#include "SingleReadResponseMessage.h"
#include "WriteResponseMessage.h"
#include "RangeQueryMessage.h"
#include "Mutex.h"

namespace pdesmas {
  class HasResponseMessage {
    private:
      AbstractMessage* fResponseMessage;
      mutable Mutex fResponseMessageMutex;
    protected:
      AbstractMessage* GetResponseMessage() const;
    public:
      void SetResponseMessage(const RollbackMessage*);
      void SetResponseMessage(const SingleReadResponseMessage*);
      void SetResponseMessage(const WriteResponseMessage*);
      void SetResponseMessage(const RangeQueryMessage*);
  };
}

#endif /* HASRESPONSEMESSAGE_H_ */
