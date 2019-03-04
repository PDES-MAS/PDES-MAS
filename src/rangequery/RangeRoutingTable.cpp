#include "RangeRoutingTable.h"
#include "SharedState.h"

using namespace std;
using namespace pdesmas;

RangeRoutingTable::RangeRoutingTable() {
  // Empty
}

RangeRoutingTable::~RangeRoutingTable() {
  fRangePeriodList.clear();
  fRangeUpdateList.clear();
}

void RangeRoutingTable::ClearRangeUpdates() {
  fRangeUpdateList.clear();
}

bool RangeRoutingTable::HasRangePeriod(unsigned long pTime) const {
  list<RangePeriod>::const_reverse_iterator rangePeriodListReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodListReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodListReverseIterator->GetTime() > pTime) ++rangePeriodListReverseIterator;
    else return true;
  }
  return false;
}

Range* RangeRoutingTable::GetRangeCopy(unsigned long pTime) const {
  list<RangePeriod>::const_reverse_iterator rangePeriodListReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodListReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodListReverseIterator->GetTime() > pTime) ++rangePeriodListReverseIterator;
    else break;
  }
  if (rangePeriodListReverseIterator != fRangePeriodList.rend())
    return rangePeriodListReverseIterator->GetRangeCopy();
  return NULL;
}

void RangeRoutingTable::AddRangeQueryInfo(unsigned long pTime, unsigned long pIdentifier, const Range& pRange, bool pBlockStatus, const LpId& pOriginalAlp) {
  list<RangePeriod>::reverse_iterator rangePeriodListReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodListReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodListReverseIterator->GetTime() > pTime) ++rangePeriodListReverseIterator;
    else break;
  }
  if (rangePeriodListReverseIterator != fRangePeriodList.rend())
    rangePeriodListReverseIterator->AddToRangeQueryInfoList(pIdentifier, pTime, pRange, pBlockStatus, pOriginalAlp);
}

bool RangeRoutingTable::GetBlockStatus(unsigned long pTime, unsigned long pIdentifier) const {
  list<RangePeriod>::const_reverse_iterator rangePeriodListReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodListReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodListReverseIterator->GetTime() > pTime) ++rangePeriodListReverseIterator;
    else break;
  }
  if (rangePeriodListReverseIterator != fRangePeriodList.rend())
    return rangePeriodListReverseIterator->GetBlockStatus(pIdentifier);
  return true;
}

void RangeRoutingTable::DeleteRangeQueryInfo(unsigned long pTime, unsigned long pIdentifier) {
  list<RangePeriod>::reverse_iterator rangePeriodReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodReverseIterator->GetTime() > pTime) ++rangePeriodReverseIterator;
    else break;
  }
  if (rangePeriodReverseIterator != fRangePeriodList.rend())
    rangePeriodReverseIterator->DeleteFromRangeQueryInfoListByIdentifier(pIdentifier);
}

void RangeRoutingTable::InsertRangePeriod(unsigned long pTime, const RangePeriod& pRangePeriod) {
  list<RangePeriod>::iterator rangePeriodListIterator = fRangePeriodList.begin();
  while (rangePeriodListIterator != fRangePeriodList.end()) {
    if (rangePeriodListIterator->GetTime() <= pTime) ++rangePeriodListIterator;
    else break;
  }
  fRangePeriodList.insert(rangePeriodListIterator, pRangePeriod);
}

void RangeRoutingTable::DeleteRangePeriod(unsigned long pTime) {
  list<RangePeriod>::iterator rangePeriodListIterator = fRangePeriodList.begin();
  while (rangePeriodListIterator != fRangePeriodList.end()) {
    if (rangePeriodListIterator->GetTime() != pTime) ++rangePeriodListIterator;
    else break;
  }
  if (rangePeriodListIterator != fRangePeriodList.end()) fRangePeriodList.erase(rangePeriodListIterator);
}

void RangeRoutingTable::ClearRangePeriods(unsigned long pTime) {
  list<RangePeriod>::iterator rangePeriodListIterator = fRangePeriodList.begin();
  list<RangePeriod>::reverse_iterator rangePeriodListReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodListReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodListReverseIterator->GetTime() > pTime) ++rangePeriodListReverseIterator;
    else break;
  }
  if (rangePeriodListReverseIterator == fRangePeriodList.rend()) return;
  if (rangePeriodListReverseIterator->GetTime() != pTime) {
    rangePeriodListReverseIterator->SetTime(pTime);
    rangePeriodListReverseIterator->DeleteFromRangeQueryInfoListByTime(pTime);
  }
  while (rangePeriodListIterator != fRangePeriodList.end()) {
    if (rangePeriodListIterator->GetTime() == pTime) break;
    rangePeriodListIterator = fRangePeriodList.erase(rangePeriodListIterator);
  }
  return;
}

void RangeRoutingTable::ProcessRangeUpdate(unsigned long pTime, const Range* pRange, RollbackList& pRollbackList) {
  list<RangePeriod>::reverse_iterator rangePeriodListReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodListReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodListReverseIterator->GetTime() > pTime) ++rangePeriodListReverseIterator;
    else break;
  }
  if (rangePeriodListReverseIterator == fRangePeriodList.rend()) {
    InsertRangePeriod(pTime, RangePeriod(pTime, pRange));
    fRangeUpdateList.push_back(RangeUpdates(pTime, pRange, NULL));
    return;
  }
  rangePeriodListReverseIterator->PopulateRollbackList(pRollbackList, pTime, pRange);
  Range* oldRange = rangePeriodListReverseIterator->GetRangeCopy();
  if (pRange && oldRange) {
    if (*pRange != *oldRange) {
      if (rangePeriodListReverseIterator->GetTime() != pTime) {
        RangePeriod newRangePeriod(pTime, pRange);
        rangePeriodListReverseIterator->MigrateRangeQueryInfo(newRangePeriod, pTime);
        InsertRangePeriod(pTime, newRangePeriod);
      } else {
        rangePeriodListReverseIterator->SetRange(pRange);
      }
      fRangeUpdateList.push_back(RangeUpdates(pTime, pRange, oldRange));
    }
  } else {
    rangePeriodListReverseIterator->SetRange(pRange);
    fRangeUpdateList.push_back(RangeUpdates(pTime, pRange, oldRange));
  }
  // Delete old range copy
  if (oldRange) delete oldRange;
}

void RangeRoutingTable::Update(const Point* pOldValue, const Point* pNewValue,
    unsigned long pTime, const Range* pNewRange, const SharedState* pSharedState,
    RollbackList& pRollbackList, unsigned long pEndTime) {
  list<RangePeriod>::reverse_iterator rangePeriodListReverseIterator = fRangePeriodList.rbegin();
  while (rangePeriodListReverseIterator != fRangePeriodList.rend()) {
    if (rangePeriodListReverseIterator->GetTime() > pTime) ++rangePeriodListReverseIterator;
    else break;
  }
  if (rangePeriodListReverseIterator == fRangePeriodList.rend()) {
    InsertRangePeriod(pTime, RangePeriod(pTime, pNewRange));
    fRangeUpdateList.push_back(RangeUpdates(pTime, pNewRange, NULL));
    return;
  }
  rangePeriodListReverseIterator->PopulateRollbackList(pOldValue , pNewValue, pRollbackList, pTime);
  Range* oldRange = rangePeriodListReverseIterator->GetRangeCopy();
  if (pNewRange && oldRange) {
    if (*pNewRange != *oldRange) {
      if (rangePeriodListReverseIterator->GetTime() != pTime) {
        RangePeriod newRangePeriod(pTime, pNewRange);
        rangePeriodListReverseIterator->MigrateRangeQueryInfo(newRangePeriod, pTime);
        InsertRangePeriod(pTime, newRangePeriod);
      } else {
        rangePeriodListReverseIterator->SetRange(pNewRange);
      }
      fRangeUpdateList.push_back(RangeUpdates(pTime, pNewRange, oldRange));
    }
  } else {
    rangePeriodListReverseIterator->SetRange(pNewRange);
    fRangeUpdateList.push_back(RangeUpdates(pTime, pNewRange, oldRange));
  }
  // Delete old range copy
  if (oldRange) delete oldRange;
  if (fRangePeriodList.size() > 1
      && fRangePeriodList.rbegin()->GetTime() != pTime
      && fRangePeriodList.rbegin() != fRangePeriodList.rend()) {
    list<RangePeriod>::iterator rangePeriodListIterator = fRangePeriodList.begin();
    while (rangePeriodListIterator != fRangePeriodList.end()) {
      if (rangePeriodListIterator->GetTime() <= pTime) ++rangePeriodListIterator;
      else break;
    }
    if (rangePeriodListIterator != fRangePeriodList.end()) {
      rangePeriodListIterator++;
      while(rangePeriodListIterator != fRangePeriodList.end()) {
        Range* newRange = NULL;
        if (rangePeriodListIterator->GetTime() < pEndTime) {
          newRange = pSharedState->RecalculateRange(rangePeriodListIterator->GetTime());
          rangePeriodListIterator->PopulateRollbackList(pOldValue, pNewValue, pRollbackList);
        } else break;
        Range* oldRange = rangePeriodListIterator->GetRangeCopy();
        if (newRange && oldRange) {
          if (oldRange != newRange) {
            rangePeriodListIterator->SetRange(newRange);
            fRangeUpdateList.push_back(RangeUpdates(pTime, newRange, oldRange));
          }
        } else {
          rangePeriodListIterator->SetRange(newRange);
          fRangeUpdateList.push_back(RangeUpdates(pTime, newRange, oldRange));
        }
        if (oldRange) delete oldRange;
        if (newRange) delete newRange;
        ++rangePeriodListIterator;
      }
    }
  }
  return;
}

const list<RangeUpdates>& RangeRoutingTable::GetRangeUpdateList() const {
  return fRangeUpdateList;
}
