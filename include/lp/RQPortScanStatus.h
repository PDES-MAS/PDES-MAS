#ifndef _RQPORTSCANSTATUS_H_
#define _RQPORTSCANSTATUS_H_

#include "Serialisable.h"
#include "SsvId.h"
#include "Value.h"
#include "Point.h"
#include "Direction.h"
#include "SerialisableMap.h"

using namespace std;

namespace pdesmas {
  enum ScanStatus {
    AVOID_SCAN, NOT_SCANNED, SCANNED
  };

  class RQPortScanStatus {
    private:
      Direction fOriginPort;
      unsigned int fNumberOfHops;
      map<Direction, ScanStatus> fPortScanStatusMap;
      SerialisableMap<SsvId, Value<Point> > fSSVValueMap;
    public:
      RQPortScanStatus(Direction, unsigned int, const SerialisableMap<SsvId, Value<Point> >&);
      ~RQPortScanStatus();

      void SetScanStatus(Direction, ScanStatus);
      ScanStatus GetScanStatus(Direction) const;
      void AddToSSVValueMap(const SerialisableMap<SsvId, Value<Point> >& );
      const SerialisableMap<SsvId, Value<Point> >& GetSSVValueMap() const;
      void SetOriginPort(Direction);
      Direction GetOriginPort() const;
      int GetNumberofHops() const;
      void IncreaseNumberOfHops(unsigned int);
  };
}
#endif
