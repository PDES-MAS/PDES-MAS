/*
 * HasRange.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasRange.h"

using namespace pdesmas;

const Range& HasRange::GetRange() const {
  return fRange;
}

void HasRange::SetRange(const Range& pRange) {
  fRange = pRange;
}
