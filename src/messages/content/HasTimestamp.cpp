/*
 * HasTimestamp.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasTimestamp.h"

using namespace pdesmas;

unsigned long HasTimestamp::GetTimestamp() const {
  return fTimestamp;
}

void HasTimestamp::SetTimestamp(unsigned long pTimestamp) {
  fTimestamp = pTimestamp;
}
