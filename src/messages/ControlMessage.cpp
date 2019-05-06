#include "ControlMessage.h"
#include "Lp.h"
#include "Log.h"

using namespace pdesmas;

ControlMessage::ControlMessage() {
  // Empty constructor
}

ControlMessage::~ControlMessage() {
  // Empty deconstructor
}

void ControlMessage::SendToLp(Lp *pLp) const {
  pLp->fSendControlMessageQueue->QueueMessage((AbstractMessage*) this);
  pLp->SignalSend();
}

void ControlMessage::ReceiveToLp(Lp *pLp) const {
  pLp->fReceiveMessageQueue->QueueMessage((AbstractMessage*) this);
}
