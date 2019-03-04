#ifndef GVTMESSAGE_H_
#define GVTMESSAGE_H_

#include "ControlMessage.h"

namespace pdesmas {
  class GvtMessage: public ControlMessage {
    public:
      GvtMessage();
      virtual ~GvtMessage();
  };
}
#endif
