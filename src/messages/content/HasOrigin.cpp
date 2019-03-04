/*
 * HasOrigin.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasOrigin.h"

using namespace pdesmas;

unsigned int HasOrigin::GetOrigin() const{
  return fOrigin;
}

void HasOrigin::SetOrigin(unsigned int pOrigin) {
  fOrigin = pOrigin;
}
