/*
 * HasOriginalAgent.cpp
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasOriginalAgent.h"

using namespace pdesmas;

const LpId &HasOriginalAgent::GetOriginalAgent() const {
  return original_agent_;
}

void HasOriginalAgent::SetOriginalAgent(const LpId &original_agent) {
  original_agent_ = original_agent;
}
