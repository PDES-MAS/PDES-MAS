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

void ControlMessage::Send(Lp* pLp) const {
  pLp->fSendControlMessageQueue->QueueMessage((AbstractMessage*) this);
  pLp->SignalSend();
}

void ControlMessage::Receive(Lp* pLp) const {
  pLp->fReceiveMessageQueue->QueueMessage((AbstractMessage*) this);
}
