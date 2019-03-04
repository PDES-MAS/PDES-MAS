/*
 * HasDestination.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasDestination.h"

using namespace pdesmas;

unsigned int HasDestination::GetDestination() const {
  return fDestination;
}

void HasDestination::SetDestination(unsigned int pDestination) {
  fDestination = pDestination;
}
