/*
 * HasMessageCount.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasMessageCount.h"

using namespace pdesmas;

int HasMessageCount::GetMessageCount(unsigned int pLpId) const {
  SerialisableMap<unsigned int, int>::const_iterator iter = fMessageCount.find(pLpId);
  if (iter != fMessageCount.end()) return iter->second;
  return -1;
}

void HasMessageCount::SetMessageCount(unsigned int pLpId, int pCount) {
  fMessageCount[pLpId] = pCount;
}
