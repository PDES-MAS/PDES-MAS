#include "RangePeriod.h"
using namespace std;
using namespace pdesmas;

RangePeriod::RangePeriod() {
  fRange = NULL;
}

RangePeriod::RangePeriod(unsigned long pTime, const Range* pRange) {
  fTime = pTime;
  fRange = NULL;
  if (pRange) {
    fRange = new Range(*pRange);
  }
}

RangePeriod::RangePeriod(const RangePeriod& pRangePeriod) {
  fTime = pRangePeriod.fTime;
  fRange = NULL;
  if (pRangePeriod.fRange != NULL)
    fRange = new Range(*(pRangePeriod.fRange));
  list<RangePeriodListEntry>::const_iterator rangePeriodListEntryIterator = pRangePeriod.fRangeQueryInfoList.begin();
  while (rangePeriodListEntryIterator != pRangePeriod.fRangeQueryInfoList.end()) {
    fRangeQueryInfoList.push_back(*rangePeriodListEntryIterator);
    ++rangePeriodListEntryIterator;
  }
}

RangePeriod::~RangePeriod() {
  if (fRange != NULL) {
    delete fRange;
    fRange = NULL;
  }
  fRangeQueryInfoList.clear();
}

unsigned long RangePeriod::GetTime() const {
  return fTime;
}

void RangePeriod::SetTime(unsigned long pTime) {
  fTime = pTime;
}

Range* RangePeriod::GetRangeCopy() const {
  if (fRange == NULL)
    return NULL;
  return new Range(*fRange);
}

void RangePeriod::SetRange(const Range* pRange) {
  if (fRange == NULL) {
    // fRange is NULL
    if (pRange == NULL) {
      // Both are NULL, nothing to do
      return;
    }
    // pRange is not NULL, fRange is, assign a copy
    fRange = new Range(*pRange);
    return;
  }
  // fRange is not null, delete the memory
  delete fRange;
  // Assign NULL
  fRange = NULL;
  // If pRange is not null, assign a copy
  if (pRange != NULL)
    fRange = new Range(*pRange);
}

void RangePeriod::AddToRangeQueryInfoList(unsigned long pIdentifier, unsigned long pTime, const Range pRange, bool pBlockStatus, const LpId& pOriginalAlp) {
  fRangeQueryInfoList.push_back(RangePeriodListEntry(pIdentifier, pTime, pRange, pBlockStatus, pOriginalAlp));
}

bool RangePeriod::GetBlockStatus(unsigned long pIdentifier) const {
  list<RangePeriodListEntry>::const_iterator rangeQueryInfoListIterator = fRangeQueryInfoList.begin();
  while (rangeQueryInfoListIterator != fRangeQueryInfoList.end()) {
    if (rangeQueryInfoListIterator->GetIdentifier()) return rangeQueryInfoListIterator->GetBlockStatus();
    else ++rangeQueryInfoListIterator;
  }
  return true;
}

void RangePeriod::DeleteFromRangeQueryInfoListByIdentifier(unsigned long pIdentifier) {
  list<RangePeriodListEntry>::iterator rangeQueryInfoListIterator = fRangeQueryInfoList.begin();
  while (rangeQueryInfoListIterator != fRangeQueryInfoList.end()) {
    if(rangeQueryInfoListIterator->GetIdentifier() == pIdentifier) rangeQueryInfoListIterator = fRangeQueryInfoList.erase(rangeQueryInfoListIterator);
    else ++rangeQueryInfoListIterator;
  }
}

void RangePeriod::DeleteFromRangeQueryInfoListByTime(unsigned long pTime) {
  list<RangePeriodListEntry>::iterator rangeQueryInfoListIterator = fRangeQueryInfoList.begin();
  while (rangeQueryInfoListIterator != fRangeQueryInfoList.end()) {
    if (rangeQueryInfoListIterator->GetTime() < pTime) rangeQueryInfoListIterator = fRangeQueryInfoList.erase(rangeQueryInfoListIterator);
    else ++rangeQueryInfoListIterator;
  }
}

void RangePeriod::PopulateRollbackList(const Point* pOldValue, const Point* pNewValue, RollbackList& pRollbackList) const {
  list<RangePeriodListEntry>::const_iterator rangeQueryInfoListIterator = fRangeQueryInfoList.begin();
  while (rangeQueryInfoListIterator != fRangeQueryInfoList.end()) {
    if ((pOldValue != NULL && rangeQueryInfoListIterator->GetRange().IsValueOverlapping(*pOldValue))
      || (pNewValue != NULL && rangeQueryInfoListIterator->GetRange().IsValueOverlapping(*pNewValue)))
      pRollbackList.AddLp(rangeQueryInfoListIterator->GetOriginalAlp(), rangeQueryInfoListIterator->GetTime());
    ++rangeQueryInfoListIterator;
  }
}

void RangePeriod::PopulateRollbackList(const Point* pOldValue, const Point* pNewValue, RollbackList& pRollbackList, unsigned long pTime) const {
  list<RangePeriodListEntry>::const_iterator rangeQueryInfoListIterator = fRangeQueryInfoList.begin();
  while (rangeQueryInfoListIterator != fRangeQueryInfoList.end()) {
    if (rangeQueryInfoListIterator->GetTime() > pTime) {
      if ((pOldValue != NULL && rangeQueryInfoListIterator->GetRange().IsValueOverlapping(*pOldValue))
          || (pNewValue != NULL && rangeQueryInfoListIterator->GetRange().IsValueOverlapping(*pNewValue)))
        pRollbackList.AddLp(rangeQueryInfoListIterator->GetOriginalAlp(), rangeQueryInfoListIterator->GetTime());
    }
    ++rangeQueryInfoListIterator;
  }
}

void RangePeriod::PopulateRollbackList(RollbackList& pRollbackList, unsigned long pTime, const Range* pRange) const {
  list<RangePeriodListEntry>::const_iterator rangeQueryInfoListIterator = fRangeQueryInfoList.begin();
  while (rangeQueryInfoListIterator != fRangeQueryInfoList.end()) {
    if (rangeQueryInfoListIterator->GetTime() > pTime) {
      if (rangeQueryInfoListIterator->GetBlockStatus() == true) {
        if (pRange != NULL && pRange->IsRangeOverlapping(rangeQueryInfoListIterator->GetRange())) {
          pRollbackList.AddLp(rangeQueryInfoListIterator->GetOriginalAlp(), rangeQueryInfoListIterator->GetTime());
        }
      }
    }
    ++rangeQueryInfoListIterator;
  }
}

void RangePeriod::MigrateRangeQueryInfo(RangePeriod& pRangePeriod, unsigned long pTime) {
  list<RangePeriodListEntry>::iterator rangeQueryInfoListIterator = fRangeQueryInfoList.begin();
  while (rangeQueryInfoListIterator != fRangeQueryInfoList.end()) {
    if (rangeQueryInfoListIterator->GetTime() > pTime) {
      pRangePeriod.AddToRangeQueryInfoList(rangeQueryInfoListIterator->GetIdentifier(), rangeQueryInfoListIterator->GetTime(), rangeQueryInfoListIterator->GetRange(), rangeQueryInfoListIterator->GetBlockStatus(), rangeQueryInfoListIterator->GetOriginalAlp());
      rangeQueryInfoListIterator = fRangeQueryInfoList.erase(rangeQueryInfoListIterator);
    } else ++rangeQueryInfoListIterator;
  }
}

