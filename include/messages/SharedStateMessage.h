/****************************************
 SharedStateMessage.h

 Author: Michael Lees (mhl)
 Date: 13/04/2005

 Description: This is an abstract base class which all messages
 operating on the shared state inherit from. These messages are,
 ReadMessage
 WriteMessage
 AddMessage
 DelMessage

 This class adds the virtual method GenerateAntiMessage, which
 generates an equivalent AntiMessage from the normal non-antimessage

 Revisions:

 ******************************************/
#ifndef SHAREDSTATEMESSAGE_H_
#define SHAREDSTATEMESSAGE_H_

#include "SimulationMessage.h"
#include "HasNumberOfHops.h"
#include "HasIdentifier.h"
#include "AntiMessage.h"
#include "HasOriginalAgent.h"

namespace pdesmas {
  class SharedStateMessage: public SimulationMessage,
      public HasNumberOfHops,
      public HasIdentifier,
      public HasOriginalAgent {
    public:
      SharedStateMessage();
      virtual ~SharedStateMessage();
  };
}
#endif
