#include "RouteTable.h"
#include "Initialisor.h"
#include <math.h>
#include "Log.h"
using namespace pdesmas;
using namespace std;

RouteTable::RouteTable() {
  // Nothing to do
}

RouteTable::~RouteTable() {
  fRankToDirectionMap.clear();
  fDirectionToRankMap.clear();
  fSSVIDToDirectionMap.clear();
}

RouteTable::RouteTable(unsigned int pLpRank, unsigned int pNumberOfClps, const Initialisor* pInitialisor) {
  ForwardingTable forwardingTable = ForwardingTable(pLpRank, pNumberOfClps);
  map<unsigned int, list<SsvId> > clpToSsvMap = pInitialisor->GetClpToSsvMap();
  map<unsigned int, list<SsvId> >::iterator clpIdSsvIdMapIterator;
  for(clpIdSsvIdMapIterator = clpToSsvMap.begin(); clpIdSsvIdMapIterator != clpToSsvMap.end(); ++clpIdSsvIdMapIterator) {
    list<SsvId>::iterator ssvIdListIterator;
    for(ssvIdListIterator = clpIdSsvIdMapIterator->second.begin(); ssvIdListIterator != clpIdSsvIdMapIterator->second.end(); ++ssvIdListIterator) {
      if (clpIdSsvIdMapIterator->first == pLpRank) {
        fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, HERE));
      } else {
        RoutingInfo routingInfo = forwardingTable.GetRoutingInfo(clpIdSsvIdMapIterator->first);
        unsigned int nextClp = routingInfo.GetNextNodeRank();
        if (nextClp == ((pLpRank - 1) / 2)) {
          fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, UP));
        } else if (nextClp == ((pLpRank * 2) + 1)) {
          fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, LEFT));
        } else if (nextClp == ((pLpRank * 2) + 2)) {
          fSSVIDToDirectionMap.insert(make_pair(*ssvIdListIterator, RIGHT));
        } else {
          LOG(logWARNING) << "RouteTable::RouteTable# Could not find direction for SSVID while initialising: SSVID: " << *ssvIdListIterator << ", next CLP: " << nextClp << ", this LP: " << pLpRank;
        }
      }
    }
  }
  fRankToDirectionMap[pLpRank] = HERE;
  Direction direction;
  if (pLpRank == 0) direction = LEFT;
  else direction = UP;
  // For the CLPs, add the neighbours to this LP to both tables.
  for (unsigned int rank = 0; rank < pNumberOfClps; ++rank) {
    if (rank == pLpRank) continue;
    RoutingInfo routingTable = forwardingTable.GetRoutingInfo(rank);
    int hops = routingTable.GetHops();
    if (hops <= 1) {
      fRankToDirectionMap[rank] = direction;
      fDirectionToRankMap[direction] = rank;
      ++direction;
    }
  }
  // For the remaining CLPs, add the proper direction to both tables.
  for (unsigned int rank = 0; rank < pNumberOfClps; ++rank) {
    if (fRankToDirectionMap.find(rank) == fRankToDirectionMap.end()) {
      RoutingInfo routingTable = forwardingTable.GetRoutingInfo(rank);
      int hops = routingTable.GetHops();
      if (hops > 1) {
        Direction newDirection = fRankToDirectionMap[routingTable.GetNextNodeRank()];
        fRankToDirectionMap[rank] = newDirection;
      }
    }
  }
  // For the ALPs, add the proper direction to both tables.
  map<unsigned int, unsigned int> alpToClpMap = pInitialisor->GetAlpToClpMap();
  for (unsigned int rank = pNumberOfClps; rank < (pNumberOfClps + alpToClpMap.size()); ++rank) {
    if (alpToClpMap[rank] == pLpRank) {
      fRankToDirectionMap[rank] = direction;
      fDirectionToRankMap[direction] = rank;
      ++direction;
    } else {
      Direction newDirection = fRankToDirectionMap[alpToClpMap[rank]];
      fRankToDirectionMap[rank] = newDirection;
    }
  }
}

Direction RouteTable::GetDirectionFromSsvId(const SsvId& pSSVID) const {
  map<SsvId, Direction>::const_iterator iter = fSSVIDToDirectionMap.find(pSSVID);
  if (iter != fSSVIDToDirectionMap.end()) return iter->second;
  LOG(logERROR) << "RouteTable::GetDirectionFromSsvId# Could not find direction for SsvId: " << pSSVID;
  return DIRECTION_SIZE;
}

Direction RouteTable::GetDirectionFromRank(unsigned int pRank) const {
  map<unsigned int, Direction>::const_iterator iter = fRankToDirectionMap.find(pRank);
  if (iter != fRankToDirectionMap.end()) return iter->second;
  LOG(logERROR) << "RouteTable::GetDirectionFromRank# Could not find direction for rank: " << pRank;
  exit(1);
}

unsigned int RouteTable::GetRankFromDirection(const Direction pDirection) const {
  map<Direction, unsigned int>::const_iterator iter = fDirectionToRankMap.find(pDirection);
  if (iter != fDirectionToRankMap.end()) return iter->second;
  LOG(logERROR) << "RouteTable::GetRankFromDirection# Could not find rank for direction" << pDirection;
  exit(1);
}

void RouteTable::SetSsvIdHost(SsvId pSSVID, Direction pDirection) {
  fSSVIDToDirectionMap[pSSVID] = pDirection;
}

