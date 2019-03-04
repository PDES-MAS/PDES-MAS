#include "SimulationMessage.h"
#include "Lp.h"

using namespace pdesmas;

SimulationMessage::SimulationMessage() {
  // Empty constructor
}

SimulationMessage::~SimulationMessage() {
  // Empty deconstructor
}

void SimulationMessage::Send(Lp* pLp) const {
  pLp->fSendMessageQueue->QueueMessage((AbstractMessage*) this);
  pLp->SignalSend();
}

void SimulationMessage::Receive(Lp* pLp) const {
  pLp->fReceiveMessageQueue->QueueMessage((AbstractMessage*) this);
}
