#ifndef ANTIMESSAGE_H_
#define ANTIMESSAGE_H_

#include "SimulationMessage.h"
#include "HasNumberOfHops.h"
#include "HasRollbackTag.h"
#include "HasOriginalAlp.h"

namespace pdesmas {
  class AntiMessage: public SimulationMessage,
      public HasNumberOfHops,
      public HasRollbackTag,
      public HasOriginalAlp {

    public:
      AntiMessage();
      virtual ~AntiMessage();
  };
}
#endif
