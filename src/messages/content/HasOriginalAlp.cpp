/*
 * HasOriginalAlp.cpp
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasOriginalAlp.h"

using namespace pdesmas;

const LpId& HasOriginalAlp::GetOriginalAlp() const {
  return fOriginalAlp;
}

void HasOriginalAlp::SetOriginalAlp(const LpId& pOriginalAlp) {
  fOriginalAlp = pOriginalAlp;
}
