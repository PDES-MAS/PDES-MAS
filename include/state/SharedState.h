#ifndef _SHAREDSTATE_H_
#define _SHAREDSTATE_H_

#include "StateVariable.h"
#include "SerialisableList.h"
#include "SerialisableMap.h"
#include "Direction.h"
#include "Value.h"
#include "SsvId.h"
#include "Range.h"
#include "Log.h"
#include "RangeRoutingTable.h"
#include "AccessCostCalculator.h"

using namespace std;
namespace pdesmas {
  class SharedState {
    private:
      map<SsvId, StateVariable> fStateVariableMap;
      RangeRoutingTable* fRangeRoutingTable;
      AccessCostCalculator* fAccessCostCalculator;

      bool ContainsVariable(const SsvId&);
    public:
      SharedState();
      ~SharedState();

      void SetRangeRoutingTable(RangeRoutingTable*);
      void SetAccessCostCalculator(AccessCostCalculator*);
      void UpdateAccessCount(const SsvId&, Direction, unsigned long);

      void Add(const SsvId&, const AbstractValue*, unsigned long, const LpId&);
      void Insert(const SsvId&, const StateVariable&, RollbackList&);
      void Delete(const SsvId&);
      StateVariable GetCopy(const SsvId&);

      AbstractValue* Read(const SsvId&, const LpId&, unsigned long);
      void WriteWithRollback(const SsvId&, const LpId&, const AbstractValue*, unsigned long, WriteStatus&, RollbackList&);
      SerialisableMap<SsvId, Value<Point> > RangeRead(const Range&, Direction, unsigned long, unsigned long);
      Range* RecalculateRange(unsigned long) const;

      void RollbackWrite(const SsvId&, const LpId&, unsigned long, RollbackList&);
      void RollbackRead(const SsvId&, const LpId&, unsigned long);

      void RemoveWritePeriods(unsigned long);
      void RemoveWritePeriodList(const SsvId&, RollbackList&);
  };
}
#endif
