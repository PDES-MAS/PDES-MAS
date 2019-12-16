/*
 * HasSendList.h
 *
 *  Created on: 1 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef HASSENDLIST_H_
#define HASSENDLIST_H_

#include <list>
#include "SharedStateMessage.h"
#include "SingleReadMessage.h"
#include "WriteMessage.h"
#include "RangeQueryMessage.h"

namespace pdesmas {
  class HasSendList {
    private:
      list<SharedStateMessage*> fSendList;

    protected:
      void AddToSendList(const SingleReadMessage*);
      void AddToSendList(const WriteMessage*);
      void AddToSendList(const RangeQueryMessage*);

    bool RemoveFromSendList(unsigned long message_id);
      list<SharedStateMessage*> RollbackSendList(unsigned long, const LpId&);
      void ClearSendList(unsigned long);
  };
}
#endif /* HASSENDLIST_H_ */
