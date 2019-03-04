/*
 * HasMessageCount.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASMESSAGECOUNT_H_
#define HASMESSAGECOUNT_H_

#include "SerialisableMap.h"

namespace pdesmas {
  class HasMessageCount {
    protected:
      SerialisableMap<unsigned int, int> fMessageCount;
    public:
      int GetMessageCount(unsigned int) const;
      void SetMessageCount(unsigned int, int);
  };
}

#endif /* HASMESSAGECOUNT_H_ */
