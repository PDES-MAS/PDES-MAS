#ifndef _RANGEROUTINGTABLE_H_ 
#define _RANGEROUTINGTABLE_H_

#include <list>
#include "Lp.h"
#include "Range.h"
#include "Serialisable.h"
#include "RangeQueryMessage.h"
#include "AbstractValue.h"
#include "LpId.h"
#include "RangeQueryAntiMessage.h"
#include "RangePeriod.h"
#include "RangeUpdates.h"
#include "RangeUpdateMessage.h"

using namespace std;

namespace pdesmas {

#define LOCAL_PORT HERE
  enum BlockStatus {
    BLOCKED, NOT_BLOCKED, AVOID
  };

  class SharedState;

  // This class maintains the routing table of range information at all ports
  // It also maintains the list of Range Query messages that has been blocked at any port.
  class RangeRoutingTable {
    private:
      list<RangePeriod> fRangePeriodList;
      list<RangeUpdates> fRangeUpdateList;

    public:
      RangeRoutingTable();
      ~RangeRoutingTable();

      bool HasRangePeriod(unsigned long) const;
      Range* GetRangeCopy(unsigned long) const;
      bool GetBlockStatus(unsigned long, unsigned long) const;
      void AddRangeQueryInfo(unsigned long, unsigned long, const Range&, bool, const LpId&);
      void DeleteRangeQueryInfo(unsigned long, unsigned long);
      void InsertRangePeriod(unsigned long, const RangePeriod&);
      void DeleteRangePeriod(unsigned long);
      void ClearRangePeriods(unsigned long);

      // TODO combine clearing range updates into process/update/get?
      const list<RangeUpdates>& GetRangeUpdateList() const;
      void ClearRangeUpdates();
      void ProcessRangeUpdate(unsigned long, const Range*, RollbackList&);
      void Update(const Point*, const Point*, unsigned long, const Range*, const SharedState*, RollbackList&, unsigned long);
  };
}
#endif

