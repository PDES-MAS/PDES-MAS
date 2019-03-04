/*
 * HasRollbackTagList.h
 *
 *  Created on: 12 Apr 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASROLLBACKTAGLIST_H_
#define HASROLLBACKTAGLIST_H_

#include <list>
#include "RollbackTag.h"

namespace pdesmas {
  class HasRollbackTagList {
    private:
      list<RollbackTag> fRollbackTagList;
    protected:
      bool CheckRollbackTagList(const RollbackTag);
      void ClearRollbackTagList(unsigned long);
  };
}

#endif /* HASROLLBACKTAGLIST_H_ */
