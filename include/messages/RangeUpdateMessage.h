#ifndef RANGEUPDATEMESSAGE_H_
#define RANGEUPDATEMESSAGE_H_

#include "LoadBalancingMessage.h"
#include "HasTimestamp.h"
#include "HasRange.h"

namespace pdesmas {
  class RangeUpdateMessage: public LoadBalancingMessage,
      public HasTimestamp,
      public HasRange {
    private:
      static AbstractMessage* CreateInstance();

    public:
      RangeUpdateMessage();
      virtual ~RangeUpdateMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif

