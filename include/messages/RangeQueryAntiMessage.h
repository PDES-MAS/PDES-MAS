#ifndef RANGEQUERYANTIMESSAGE_H_
#define RANGEQUERYANTIMESSAGE_H_

#include "AntiMessage.h"
#include "HasRange.h"
#include "HasIdentifier.h"

namespace pdesmas {
  class RangeQueryAntiMessage: public AntiMessage,
      public HasRange,
      public HasIdentifier {
    private:
      static AbstractMessage* CreateInstance();

    public:
      RangeQueryAntiMessage();
      virtual ~RangeQueryAntiMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif

