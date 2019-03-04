/*
 * HasSSVID.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASSSVID_H_
#define HASSSVID_H_

#include "SsvId.h"

namespace pdesmas {
  class HasSSVID {
    protected:
      SsvId fSsvId;
    public:
      const SsvId& GetSsvId() const;
      void SetSsvId(const SsvId&);
  };
}

#endif /* HASSSVID_H_ */
