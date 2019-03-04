/*
 * HasGVT.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasGVT.h"

using namespace pdesmas;

unsigned long HasGVT::GetGVT() const {
  return fGVT;
}

void HasGVT::SetGVT(unsigned long pGVT) {
  fGVT = pGVT;
}
