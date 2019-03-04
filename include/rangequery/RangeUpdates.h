#ifndef _RANGEUPDATES_H_ 
#define _RANGEUPDATES_H_

using namespace std;

#include "Range.h"
#include "Serialisable.h"

namespace pdesmas {

  class RangeUpdates {
    private:
      unsigned long fTime;
      Range* fOldRange;
      Range* fNewRange;
    public:
      RangeUpdates(unsigned long, const Range*, const Range*);
      RangeUpdates(const RangeUpdates&);
      ~RangeUpdates();

      unsigned long GetTime() const;
      Range* GetOldRange() const;
      Range* GetNewRange() const;
  };
}
#endif

