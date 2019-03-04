#include "RoutingInfo.h"

using namespace std;
using namespace pdesmas;

RoutingInfo::RoutingInfo() {
  // Nothing to do
}

RoutingInfo::RoutingInfo(unsigned int pNodeRank, unsigned int pDestination, unsigned int pNumberOfHops, unsigned int pNextNodeRank) :
  fNodeRank(pNodeRank), fDestination(pDestination), fNumberOfHops(pNumberOfHops), fNextNodeRank(pNextNodeRank) {
}

RoutingInfo::RoutingInfo(const RoutingInfo& pRoutingInfo) {
  fNodeRank = pRoutingInfo.fNodeRank;
  fDestination = pRoutingInfo.fDestination;
  fNumberOfHops = pRoutingInfo.fNumberOfHops;
  fNextNodeRank = pRoutingInfo.fNextNodeRank;
}

RoutingInfo::~RoutingInfo() {
  // Nothing to do
}

unsigned int RoutingInfo::GetNodeRank() const {
  return fNodeRank;
}

unsigned int RoutingInfo::GetDestination() const {
  return fDestination;
}

unsigned int RoutingInfo::GetHops() const {
  return fNumberOfHops;
}

unsigned int RoutingInfo::GetNextNodeRank() const {
  return fNextNodeRank;
}
