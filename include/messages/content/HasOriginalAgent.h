/*
 * HasOriginalAgent.h
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASORIGINALALP_H_
#define HASORIGINALALP_H_

#include "LpId.h"

namespace pdesmas {
  class HasOriginalAgent {
  protected:
    LpId original_agent_;
  public:
    const LpId &GetOriginalAgent() const;

    void SetOriginalAgent(const LpId &);
  };
}

#endif /* HASORIGINALALP_H_ */
