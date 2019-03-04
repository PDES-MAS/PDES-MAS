/*
 * HasMessageMinimumTime.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasMessageMinimumTime.h"

using namespace pdesmas;

unsigned long HasMessageMinimumTime::GetMessageMinimumTime() const {
  return fMessageMinimumTime;
}

void HasMessageMinimumTime::SetMessageMinimumTime(unsigned long pMessageMiniumTime) {
  fMessageMinimumTime = pMessageMiniumTime;
}
