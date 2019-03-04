/*
 * ForwardingTable.cpp
 *
 *  Created on: 14 Sep 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include "ForwardingTable.h"
#include "Direction.h"
#include "Log.h"

using namespace pdesmas;

ForwardingTable::ForwardingTable(unsigned int pClpRank, unsigned int pNumberOfClps) : fClpRank(pClpRank), fNumberOfClps(pNumberOfClps) {
  fForwardingTable.insert(make_pair(fClpRank, RoutingInfo(fClpRank, fClpRank, 0, fClpRank)));
  WalkTree(2*fClpRank + 1, 1, 2*fClpRank + 1);
  WalkTree(2*fClpRank + 2, 1, 2*fClpRank + 2);
  if (fClpRank != 0) WalkTree((fClpRank - 1) / 2, 1, (fClpRank - 1) / 2);
}

ForwardingTable::~ForwardingTable() {
  fForwardingTable.clear();
}

const RoutingInfo& ForwardingTable::GetRoutingInfo(unsigned int pLpRank) const {
  map<unsigned int, RoutingInfo>::const_iterator iter = fForwardingTable.find(pLpRank);
  if (iter != fForwardingTable.end()) return iter->second;
  LOG(logERROR) << "ForwardingTable::GetRoutingInfo# Could not get routing info, CLP: " << fClpRank << ", rank: " << pLpRank;
  exit(1);
}

void ForwardingTable::WalkTree(unsigned int pRank, unsigned int pNumberOfHops, unsigned int pNextRank) {
  if (pRank > (fNumberOfClps - 1)) return;
  map<unsigned int, RoutingInfo>::const_iterator iter = fForwardingTable.find(pRank);
  if (iter != fForwardingTable.end()) return;
  fForwardingTable.insert(make_pair(pRank, RoutingInfo(fClpRank, pRank, pNumberOfHops, pNextRank)));
  WalkTree(2*pRank + 1, pNumberOfHops + 1, pNextRank);
  WalkTree(2*pRank + 2, pNumberOfHops + 1, pNextRank);
  if ((pNextRank != 2*fClpRank + 1 && pNextRank != 2*fClpRank + 2) && pRank != 0) {
    WalkTree((pRank - 1) / 2, pNumberOfHops + 1, pNextRank);
  }
}
