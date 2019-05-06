#include "LoadBalancingMessage.h"
#include "Lp.h"

using namespace pdesmas;

LoadBalancingMessage::LoadBalancingMessage() {
  // Empty constructor
}

LoadBalancingMessage::~LoadBalancingMessage() {
  // Empty deconstructor
}

void LoadBalancingMessage::SendToLp(Lp *pLp) const {
  pLp->fSendLoadBalancingMessageQueue->QueueMessage((AbstractMessage*) this);
  pLp->SignalSend();
}

void LoadBalancingMessage::ReceiveToLp(Lp *pLp) const {
  pLp->fReceiveLoadBalancingMessageQueue->QueueMessage((AbstractMessage*) this);
}
