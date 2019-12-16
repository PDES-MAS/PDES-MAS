#ifndef ANTIMESSAGE_H_
#define ANTIMESSAGE_H_

#include "SimulationMessage.h"
#include "HasNumberOfHops.h"
#include "HasRollbackTag.h"
#include "HasOriginalAgent.h"

namespace pdesmas {
  class AntiMessage: public SimulationMessage,
      public HasNumberOfHops,
      public HasRollbackTag,
      public HasOriginalAgent {

    public:
      AntiMessage();
      virtual ~AntiMessage();
  };
}
#endif
