/*
 * RangePeriodListEntry.cpp
 *
 *  Created on: 3 Aug 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include "RangePeriodListEntry.h"

using namespace pdesmas;

RangePeriodListEntry::RangePeriodListEntry(unsigned long pIdentifier, unsigned long pTime, const Range& pRange, bool pBlockStatus, const LpId& pOriginalAlp) :
  fBlockStatus(pBlockStatus), fIdentifier(pIdentifier), fTime(pTime), fRange(pRange), fOriginalAlp(pOriginalAlp) {
  // Nothing more to do
}

RangePeriodListEntry::RangePeriodListEntry(const RangePeriodListEntry& pRangePeriodListEntry) {
  fBlockStatus = pRangePeriodListEntry.fBlockStatus;
  fIdentifier = pRangePeriodListEntry.fIdentifier;
  fTime = pRangePeriodListEntry.fTime;
  fRange = pRangePeriodListEntry.fRange;
  fOriginalAlp = pRangePeriodListEntry.fOriginalAlp;
}

RangePeriodListEntry::~RangePeriodListEntry() {
  // Nothing to do
}

bool RangePeriodListEntry::GetBlockStatus() const {
  return fBlockStatus;
}

unsigned long RangePeriodListEntry::GetIdentifier() const {
  return fIdentifier;
}

unsigned long RangePeriodListEntry::GetTime() const {
  return fTime;
}

const Range& RangePeriodListEntry::GetRange() const {
  return fRange;
}

const LpId& RangePeriodListEntry::GetOriginalAlp() const {
  return fOriginalAlp;
}
