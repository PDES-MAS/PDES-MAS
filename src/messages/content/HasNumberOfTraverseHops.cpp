/*
 * HasNumberOfTraverseHops.cpp
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasNumberOfTraverseHops.h"

using namespace pdesmas;

unsigned int HasNumberOfTraverseHops::GetNumberOfTraverseHops() const {
  return fNumberOfTraverseHops;
}

void HasNumberOfTraverseHops::SetNumberOfTraverseHops(unsigned int pTrHops) {
  fNumberOfTraverseHops = pTrHops;
}

void HasNumberOfTraverseHops::IncrementNumberOfTraverseHops() {
  fNumberOfTraverseHops++;
}
