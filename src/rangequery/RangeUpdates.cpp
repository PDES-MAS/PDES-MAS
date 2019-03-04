/*
 * RangeUpdates.cpp
 *
 *  Created on: 4 Aug 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include "RangeUpdates.h"

using namespace pdesmas;

RangeUpdates::RangeUpdates(unsigned long pTime, const Range* pNewRange, const Range* pOldRange) {
  fTime = pTime;
  fNewRange = NULL;
  if (pNewRange) fNewRange = new Range(*pNewRange);
  fOldRange = NULL;
  if (pOldRange) fOldRange = new Range(*pOldRange);
}

RangeUpdates::RangeUpdates(const RangeUpdates& pRangeUpdates) {
  fTime = pRangeUpdates.fTime;
  fNewRange = NULL;
  if (pRangeUpdates.fNewRange) fNewRange = new Range(*(pRangeUpdates.fNewRange));
  fOldRange = NULL;
  if (pRangeUpdates.fOldRange) fOldRange = new Range(*(pRangeUpdates.fOldRange));
}

RangeUpdates::~RangeUpdates() {
  if (fNewRange) delete fNewRange;
  if (fOldRange) delete fOldRange;
}

unsigned long RangeUpdates::GetTime() const {
  return fTime;
}

Range* RangeUpdates::GetOldRange() const {
  if (fOldRange) return new Range(*fOldRange);
  return NULL;
}

Range* RangeUpdates::GetNewRange() const {
  if (fNewRange) return new Range(*fNewRange);
  return NULL;
}
