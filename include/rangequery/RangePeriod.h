#ifndef _RANGEPERIOD_H_ 
#define _RANGEPERIOD_H_

#include <list>
#include "Range.h"
#include "Serialisable.h"
#include "RangeQueryMessage.h"
#include "RangePeriodListEntry.h"
#include "RollbackList.h"

using namespace std;

namespace pdesmas {

  class RangePeriod {
    private:
      unsigned long fTime;
      Range *fRange;
      list<RangePeriodListEntry> fRangeQueryInfoList;

    public:
      RangePeriod();
      RangePeriod(unsigned long, const Range*);
      RangePeriod(const RangePeriod&);
      ~RangePeriod();

      unsigned long GetTime() const;
      void SetTime(unsigned long);
      Range* GetRangeCopy() const;
      void SetRange(const Range*);

      void AddToRangeQueryInfoList(unsigned long, unsigned long, const Range, bool, const LpId&);
      bool GetBlockStatus(unsigned long) const;
      void DeleteFromRangeQueryInfoListByIdentifier(unsigned long);
      void DeleteFromRangeQueryInfoListByTime(unsigned long);
      void PopulateRollbackList(const Point*, const Point*, RollbackList&) const;
      void PopulateRollbackList(const Point*, const Point*, RollbackList&, unsigned long) const;
      void PopulateRollbackList(RollbackList&, unsigned long, const Range*) const;
      void MigrateRangeQueryInfo(RangePeriod&, unsigned long);
  };
}
#endif

