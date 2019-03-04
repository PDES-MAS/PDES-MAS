#include "AccessCostCalculator.h"

using namespace std;
using namespace pdesmas;

AccessCostCalculator::AccessCostCalculator(int pLpId, int pNumberOfClps) {
  fIsParentCLP = ((int) ceil(log2((double) pNumberOfClps + 1))) == ((int) ceil(log2((double) pLpId + 2)));
  fPortThreshold = (unsigned long) (2 * log2(pNumberOfClps) * THRESHOLD_CONST);
  fLPThreshold = (unsigned long) ((fSSVList.size()) * fPortThreshold);
  fCLPLoad = 0;
}

AccessCostCalculator::~AccessCostCalculator() {
  fSSVList.clear();
  fSSVPortAccessMap.clear();
  fSSVPortHopMap.clear();
}

void AccessCostCalculator::InitialiseCounters(const SsvId& pSsvId) {
  if (!ContainsVariable(pSsvId)) {
    fSSVList.push_back(pSsvId);
    fLPThreshold = (unsigned long) ((fSSVList.size()) * fPortThreshold);
  }
  CounterMap initialCounterMap;
  for (size_t Size = 1; Size < DIRECTION_SIZE; ++Size) {
    initialCounterMap.insert(make_pair((Direction) Size, (unsigned long) 0));
  }
  fSSVPortHopMap[pSsvId] = initialCounterMap;
  fSSVPortAccessMap[pSsvId] = initialCounterMap;
}

unsigned long AccessCostCalculator::UpdateAccessCount(Direction pDirection, unsigned long pAccessCount, const SsvId& pSsvId) {
  if (!ContainsVariable(pSsvId)) {
    LOG(logERROR) << "AccessCostCalculator::UpdateAccessCount# Trying to update access count on non-existent variable" << pSsvId;
    return 0;
  }
  unsigned long oldAccessCount = 0;
  map<SsvId, CounterMap>::iterator ssvPortAccessMapIterator = fSSVPortAccessMap.find(pSsvId);
  if (ssvPortAccessMapIterator != fSSVPortAccessMap.end()) {
    CounterMap::iterator counterMapIterator = ssvPortAccessMapIterator->second.find(pDirection);
    if (counterMapIterator != ssvPortAccessMapIterator->second.end()) {
      oldAccessCount = counterMapIterator->second;
      counterMapIterator->second += pAccessCount;
    } else ssvPortAccessMapIterator->second.insert( make_pair(pDirection, pAccessCount));
  }
  return oldAccessCount;
}

void AccessCostCalculator::RemoveSsvAccessRecord(const SsvId& pSsvId) {
  if (!ContainsVariable(pSsvId)) {
    LOG(logERROR) << "AccessCostCalculator::RemoveSsvAccessRecord# Trying to remove access count on non-existent variable" << pSsvId;
    return;
  }
  map<SsvId, CounterMap>::iterator ssvPortAccessMapIterator = fSSVPortAccessMap.find(pSsvId);
  fSSVPortAccessMap.erase(ssvPortAccessMapIterator);
}

unsigned long AccessCostCalculator::UpdateHopCount(Direction pDirection, unsigned long pHopCount, const SsvId& pSsvId) {
  if (!ContainsVariable(pSsvId)) {
    LOG(logERROR) << "AccessCostCalculator::UpdateHopCount# Trying to update hop count on non-existent variable" << pSsvId;
    return 0;
  }
  unsigned long oldHops = 0;
  map<SsvId, CounterMap>::iterator ssvPortHopMapIterator = fSSVPortHopMap.find( pSsvId);
  if (ssvPortHopMapIterator != fSSVPortHopMap.end()) {
    CounterMap::iterator counterMapIterator = ssvPortHopMapIterator->second.find(pDirection);
    if (counterMapIterator != ssvPortHopMapIterator->second.end()) {
      oldHops = counterMapIterator->second;
      counterMapIterator->second += pHopCount;
    } else ssvPortHopMapIterator->second.insert(make_pair(pDirection, pHopCount));
  }
  return oldHops;
}

void AccessCostCalculator::RemoveSsvHopRecord(const SsvId& pSsvId) {
  if (!ContainsVariable(pSsvId)) {
    LOG(logERROR) << "AccessCostCalculator::RemoveSsvHopRecord# Trying to remove hop count on non-existent variable" << pSsvId;
    return;
  }
  map<SsvId, CounterMap>::iterator ssvPortHopMapIterator = fSSVPortHopMap.find( pSsvId);
  fSSVPortHopMap.erase(ssvPortHopMapIterator);
}

void AccessCostCalculator::RemoveSsvFromList(const SsvId& pSsvId) {
  list<SsvId>::iterator ssvListIterator = fSSVList.begin();
  if (fSSVList.size() > 0) {
    while (ssvListIterator != fSSVList.end()) {
      if (*ssvListIterator == pSsvId) {
        fSSVList.erase(ssvListIterator);
        break;
      }
      ++ssvListIterator;
    }
  }
  fLPThreshold = (unsigned long) ((fSSVList.size()) * fPortThreshold);
}

void AccessCostCalculator::UpdateLoad(unsigned long pOldHops, unsigned long pOldAccess, unsigned long pNewHops, unsigned long pNewAccess) {
  fCLPLoad += ((pNewHops - pOldHops) * (pNewAccess - pOldAccess));
}

bool AccessCostCalculator::CheckClpload() {
  if (fCLPLoad <= fLPThreshold) {
    fCLPLoad = 0;
    return false;
  }
  fCLPLoad = 0;
  return true;
}

map<Direction, list<SsvId> > AccessCostCalculator::GetMigrationMap() {
  map<Direction, list<SsvId> > MigrateSSVMap;
  list<SsvId>::iterator ssvListIterator = fSSVList.begin();
  while (ssvListIterator != fSSVList.end()) {
    long ssvAccessCost = 0;
    unsigned long ssvPortAccessCost = 0;
    long ssvHopCost = 0;
    for (int direction = 1; direction < DIRECTION_SIZE; ++direction) {
      ssvHopCost = ssvHopCost + GetHopCount((Direction) direction, *ssvListIterator);
      ssvAccessCost = ssvAccessCost + GetAccessCount((Direction) direction, *ssvListIterator);
      ssvPortAccessCost = ssvPortAccessCost + (GetAccessCount( (Direction) direction, *ssvListIterator) * GetHopCount( (Direction) direction, *ssvListIterator));
    }

    if (ssvPortAccessCost > fPortThreshold) {
      for (int direction = 1; direction < DIRECTION_SIZE; ++direction) {
        if (GetAccessCount((Direction) direction, *ssvListIterator) > (2 * (ssvAccessCost - GetAccessCount((Direction) direction, *ssvListIterator)))) {
          if (!fIsParentCLP || (Direction) direction == UP) {
            map<Direction, list<SsvId> >::iterator migrateSSVMapIterator = MigrateSSVMap.find((Direction) direction);
            if (migrateSSVMapIterator != MigrateSSVMap.end()) {
              migrateSSVMapIterator->second.push_back(*ssvListIterator);
            } else {
              list<SsvId> tempSSVList;
              tempSSVList.push_back(*ssvListIterator);
              MigrateSSVMap.insert(make_pair((Direction) direction, tempSSVList));
            }
          }
        }
      }
    }
    ++ssvListIterator;
  }
  fCLPLoad = 0;
  return MigrateSSVMap;
}

bool AccessCostCalculator::ContainsVariable(const SsvId& pSsvId) const {
  list<SsvId>::const_iterator SsvListIterator = fSSVList.begin();
  if (fSSVList.size() > 0) {
    while (SsvListIterator != fSSVList.end()) {
      if (*SsvListIterator == pSsvId) return true;
      ++SsvListIterator;
    }
  }
  return false;
}

unsigned long AccessCostCalculator::GetAccessCount(Direction pDirection,
    const SsvId& pSsvId) const {
  map<SsvId, CounterMap>::const_iterator SsvPortAccessMapIterator = fSSVPortAccessMap.find(pSsvId);
  if (SsvPortAccessMapIterator == fSSVPortAccessMap.end()) return 0;
  CounterMap::const_iterator CounterMapIterator = SsvPortAccessMapIterator->second.find(pDirection);
  if (CounterMapIterator != SsvPortAccessMapIterator->second.end()) return CounterMapIterator->second;
  return 0;
}

unsigned long AccessCostCalculator::GetHopCount(Direction pDirection,
    const SsvId& pSsvId) const {
  map<SsvId, CounterMap>::const_iterator SsvPortHopMapIterator = fSSVPortHopMap.find(pSsvId);
  if (SsvPortHopMapIterator == fSSVPortHopMap.end()) return 0;
  CounterMap::const_iterator CounterMapIterator = SsvPortHopMapIterator->second.find( pDirection);
  if (CounterMapIterator != SsvPortHopMapIterator->second.end()) return CounterMapIterator->second;
  return 0;
}
