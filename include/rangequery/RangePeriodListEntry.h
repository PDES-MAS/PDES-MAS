/*
 * RangePeriodListEntry.h
 *
 *  Created on: 3 Aug 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef RANGEPERIODLISTENTRY_H_
#define RANGEPERIODLISTENTRY_H_

#include "Range.h"
#include "LpId.h"

namespace pdesmas {
  class RangePeriodListEntry {
    private:
      bool fBlockStatus;
      unsigned long fIdentifier;
      unsigned long fTime;
      Range fRange;
      LpId fOriginalAlp;
    public:
      RangePeriodListEntry(unsigned long, unsigned long, const Range&, bool, const LpId&);
      RangePeriodListEntry(const RangePeriodListEntry&);
      ~RangePeriodListEntry();

      bool GetBlockStatus() const;
      unsigned long GetIdentifier() const;
      unsigned long GetTime() const;
      const Range& GetRange() const;
      const LpId& GetOriginalAlp() const;
  };
}

#endif /* RANGEPERIODLISTENTRY_H_ */
