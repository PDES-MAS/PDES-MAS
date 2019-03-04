/*
 * HasOriginalAlp.h
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASORIGINALALP_H_
#define HASORIGINALALP_H_

#include "LpId.h"

namespace pdesmas {
  class HasOriginalAlp {
    protected:
      LpId fOriginalAlp;
    public:
      const LpId& GetOriginalAlp() const;
      void SetOriginalAlp(const LpId&);
  };
}

#endif /* HASORIGINALALP_H_ */
