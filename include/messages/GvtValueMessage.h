#ifndef GVTVALUEMESSAGE_H_
#define GVTVALUEMESSAGE_H_

#include "GvtMessage.h"
#include "HasGVT.h"

namespace pdesmas {
  class GvtValueMessage: public GvtMessage, public HasGVT {
    private:
      static AbstractMessage* CreateInstance();

    public:
      GvtValueMessage();
      virtual ~GvtValueMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
