/*
 * HasRedMessageTime.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasRedMessageTime.h"

using namespace pdesmas;

unsigned long HasRedMessageTime::GetRedMessageTime() const {
  return fRedMessageTime;
}

void HasRedMessageTime::SetRedMessageTime(unsigned long pRedMessageTime) {
  fRedMessageTime = pRedMessageTime;
}
