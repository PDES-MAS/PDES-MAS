/*
 * HasRollbackTag.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasRollbackTag.h"

using namespace pdesmas;

const RollbackTag& HasRollbackTag::GetRollbackTag() const {
  return fRollbackTag;
}

void HasRollbackTag::SetRollbackTag(const RollbackTag& pRollbackTag) {
  fRollbackTag = pRollbackTag;
}
