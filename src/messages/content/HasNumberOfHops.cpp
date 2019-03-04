/*
 * HasNumberOfHops.cpp
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasNumberOfHops.h"

using namespace pdesmas;

unsigned long HasNumberOfHops::GetNumberOfHops() const {
  return fNumberOfHops;
}

void HasNumberOfHops::SetNumberOfHops(unsigned long pNumberOfHops) {
  fNumberOfHops = pNumberOfHops;
}

void HasNumberOfHops::IncrementNumberOfHops() {
  fNumberOfHops++;
}
