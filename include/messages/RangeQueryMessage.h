  /*!
   \brief Class for reading a single SSV

   An instance of SingleReadMessage defines a message for an ALP
   ALP reading the value of an individual Shared State Variable.
   */
#ifndef RANGEQUERYMESSAGE_H_
#define RANGEQUERYMESSAGE_H_

#include "SharedStateMessage.h"
#include "HasRange.h"
#include "HasNumberOfTraverseHops.h"
#include "HasSSVIDValueMap.h"

namespace pdesmas {
  class RangeQueryMessage: public SharedStateMessage,
      public HasRange,
      public HasNumberOfTraverseHops,
      public HasSSVIDValueMap {
    private:
      static AbstractMessage* CreateInstance();

    public:
      RangeQueryMessage();
      virtual ~RangeQueryMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif

