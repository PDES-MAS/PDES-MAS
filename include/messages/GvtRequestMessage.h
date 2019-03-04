#ifndef GVTREQUESTMESSAGE_H_
#define GVTREQUESTMESSAGE_H_

#include "GvtMessage.h"

namespace pdesmas {
  class GvtRequestMessage: public GvtMessage {
    private:
      static AbstractMessage* CreateInstance();

    public:
      GvtRequestMessage();
      virtual ~GvtRequestMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
