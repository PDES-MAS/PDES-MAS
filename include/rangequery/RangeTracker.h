#ifndef _RANGETRACKER_H_
#define _RANGETRACKER_H_

#include <map>
#include "Serialisable.h"
#include "RQPortScanStatus.h"
#include "RangeQueryMessage.h"
#include "SsvId.h"
#include "Value.h"

using namespace std;

namespace pdesmas {
  class RangeTracker {
    private:
      map<unsigned long, RQPortScanStatus> fRangeQueryPortStatus;
    public:
      RangeTracker();
      ~RangeTracker();

      bool HasEntry(unsigned long) const;
      void CreateEntry(unsigned long, const SerialisableMap<SsvId, Value<Point> >&, Direction, int);
      Direction DeleteEntry(unsigned long);

      list<Direction> GetOutstandingPorts(unsigned long) const;

      void IncreaseNumberOfHops(unsigned long, unsigned int);
      int GetNumberofHops(unsigned long) const;

      void SetPortToScanned(unsigned long, Direction);

      void AddToSSVValueMap(unsigned long, const SerialisableMap<SsvId, Value<Point> >&);
      SerialisableMap<SsvId, Value<Point> > GetCollectedSSVValueMap(unsigned long);
  };
}

#endif

