#ifndef LOADBALANCINGMESSAGE_H_
#define LOADBALANCINGMESSAGE_H_

#include "AbstractMessage.h"

namespace pdesmas {
  class LoadBalancingMessage: public AbstractMessage {
    public:
      LoadBalancingMessage();
      virtual ~LoadBalancingMessage();

      void SendToLp(Lp *) const;
      void ReceiveToLp(Lp *) const;
  };
}
#endif
