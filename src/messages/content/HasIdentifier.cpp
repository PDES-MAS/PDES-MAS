/*
 * HasIndentifier.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include "HasIdentifier.h"

using namespace pdesmas;

unsigned long HasIdentifier::GetIdentifier() const {
  return fIdentifier;
}

void HasIdentifier::SetIdentifier(unsigned long pIdentifier) {
  fIdentifier = pIdentifier;
}

