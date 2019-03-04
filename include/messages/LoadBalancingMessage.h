#ifndef LOADBALANCINGMESSAGE_H_
#define LOADBALANCINGMESSAGE_H_

#include "AbstractMessage.h"

namespace pdesmas {
  class LoadBalancingMessage: public AbstractMessage {
    public:
      LoadBalancingMessage();
      virtual ~LoadBalancingMessage();

      void Send(Lp*) const;
      void Receive(Lp*) const;
  };
}
#endif
