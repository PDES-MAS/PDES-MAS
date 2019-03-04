/*
 * HasSenderAlp.cpp
 *
 *  Created on: 16 Sep 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include "HasSenderAlp.h"

using namespace pdesmas;

unsigned HasSenderAlp::GetSenderAlp() const {
  return fSenderAlp;
}

void HasSenderAlp::SetSenderAlp(unsigned int pSenderAlp) {
  fSenderAlp = pSenderAlp;
}
