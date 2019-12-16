/*
 * Base class for all simulation message classes.
 *
 * Simulation message are blocked by windows and its timestamp is
 * considered in GVT calculation.
 */
#ifndef SIMULATIONMESSAGE_H_
#define SIMULATIONMESSAGE_H_

#include "AbstractMessage.h"
#include "HasTimestamp.h"
#include "HasMatternColour.h"

namespace pdesmas {
  class SimulationMessage: public AbstractMessage,
      public HasTimestamp,
      public HasMatternColour {
    public:
      SimulationMessage();
      virtual ~SimulationMessage();

      void SendToLp(Lp *) const;
      void ReceiveToLp(Lp *) const;
  };
}
#endif
