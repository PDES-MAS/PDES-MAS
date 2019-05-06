/*
 * Base class for all control message classes.
 *
 * A control message is NOT blocked by windows and its
 * timestamp is NOT considered in GVT calculation.
 */
#ifndef CONTROLMESSAGE_H_
#define CONTROLMESSAGE_H_

#include "AbstractMessage.h"

namespace pdesmas {
  class ControlMessage: public AbstractMessage {
    public:
      ControlMessage();
      virtual ~ControlMessage();

      void SendToLp(Lp *pLp) const;
      void ReceiveToLp(Lp *pLp) const;
  };
}
#endif
