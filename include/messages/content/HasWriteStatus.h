/*
 * HasWriteStatus.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASWRITESTATUS_H_
#define HASWRITESTATUS_H_

#include "WriteStatus.h"

namespace pdesmas {
  class HasWriteStatus {
    protected:
      WriteStatus fWriteStatus;
    public:
      WriteStatus GetWriteStatus() const;
      void SetWriteStatus(WriteStatus);
  };
}

#endif /* HASWRITESTATUS_H_ */
