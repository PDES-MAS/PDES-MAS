/*
 * HasSSVID.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasSSVID.h"

using namespace pdesmas;

const SsvId& HasSSVID::GetSsvId() const {
  return fSsvId;
}

void HasSSVID::SetSsvId(const SsvId& pSsvId) {
  fSsvId = pSsvId;
}
