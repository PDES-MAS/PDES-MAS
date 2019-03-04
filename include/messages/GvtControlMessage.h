#ifndef GVTCONTROLMESSAGE_H_
#define GVTCONTROLMESSAGE_H_

#include "GvtMessage.h"
#include "HasMessageMinimumTime.h"
#include "HasRedMessageTime.h"
#include "HasMatternCut.h"
#include "HasMessageCount.h"

namespace pdesmas {
  class GvtControlMessage: public GvtMessage,
      public HasMessageMinimumTime,
      public HasRedMessageTime,
      public HasMatternCut,
      public HasMessageCount {
    private:
      static AbstractMessage* CreateInstance();

    public:
      GvtControlMessage();
      virtual ~GvtControlMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
