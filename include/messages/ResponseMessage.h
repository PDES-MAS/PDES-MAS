  /*!
   \brief Base class of all response message classes

   A mixin class to distinguish all types of response message which is
   inherited from Simulation message
   */
#ifndef RESPONSEMESSAGE_H_
#define RESPONSEMESSAGE_H_

#include "SimulationMessage.h"
#include "HasIdentifier.h"
#include "HasOriginalAgent.h"

namespace pdesmas {
  class ResponseMessage: public SimulationMessage,
  public HasIdentifier,
  public HasOriginalAgent {
    public:
      ResponseMessage();
      virtual ~ResponseMessage();
  };
}
#endif
