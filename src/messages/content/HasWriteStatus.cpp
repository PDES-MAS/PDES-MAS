/*
 * HasWriteStatus.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasWriteStatus.h"

using namespace pdesmas;

WriteStatus HasWriteStatus::GetWriteStatus() const {
  return fWriteStatus;
}

void HasWriteStatus::SetWriteStatus(WriteStatus pWriteStatus) {
  fWriteStatus = pWriteStatus;
}
