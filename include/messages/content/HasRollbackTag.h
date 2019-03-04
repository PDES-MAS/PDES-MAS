/*
 * HasRollbackTag.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASROLLBACKTAG_H_
#define HASROLLBACKTAG_H_

#include "RollbackTag.h"

namespace pdesmas {
  class HasRollbackTag {
    protected:
      RollbackTag fRollbackTag;
    public:
      const RollbackTag& GetRollbackTag() const;
      void SetRollbackTag(const RollbackTag&);
  };
}

#endif /* HASROLLBACKTAG_H_ */
