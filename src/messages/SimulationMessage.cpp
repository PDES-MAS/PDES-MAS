#include "SimulationMessage.h"
#include "Lp.h"

using namespace pdesmas;

SimulationMessage::SimulationMessage() {
  // Empty constructor
}

SimulationMessage::~SimulationMessage() {
  // Empty deconstructor
}

void SimulationMessage::SendToLp(Lp *pLp) const {
  pLp->fSendMessageQueue->QueueMessage((AbstractMessage*) this);
  pLp->SignalSend();
}

void SimulationMessage::ReceiveToLp(Lp *pLp) const {
  pLp->fReceiveMessageQueue->QueueMessage((AbstractMessage*) this);
}
