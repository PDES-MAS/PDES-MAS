#ifndef _ROUTER_H_
#define _ROUTER_H_

/*!

 \brief Class which uses different routing tables to route messages through the tree

 Router.h

 Author: Michael Lees (mhl)
 Date: 07/07/2005

 Description: Router class which routes messages around the tree. The
 class contains two routing tables, one which takes an LpId and returns
 a direction (this is for messages going to Alps) and another which
 takes an SsvId and returns a direction (this is for messages going to
 Clps).

 Revisions:

 ******************************************/

#include "SsvId.h"
#include "AbstractMessage.h"
#include "SharedStateMessage.h"
#include "ResponseMessage.h"
#include "AntiMessage.h"
#include "GvtMessage.h"
#include "RollbackMessage.h"
#include "WriteMessage.h"
#include "SingleReadMessage.h"
#include "RouteTable.h"
#include "LoadBalancingMessage.h"
#include "StateMigrationMessage.h"
#include "RangeUpdateMessage.h"
#include "RangeQueryMessage.h"
#include "RangeQueryAntiMessage.h"
#include "EndMessage.h"
#include "GvtRequestMessage.h"
#include "SingleReadAntiMessage.h"
#include "SingleReadResponseMessage.h"
#include "WriteAntiMessage.h"
#include "WriteResponseMessage.h"
#include "GvtControlMessage.h"
#include "GvtValueMessage.h"

using namespace std;

namespace pdesmas {
  class Initialisor;

  class Router {

    private:
      unsigned int fLpRank;
      RouteTable fRouteTable;

    public:
      Router();
      Router(unsigned int, unsigned int, const Initialisor*);
      ~Router();

      bool Route(RollbackMessage*);
      bool Route(SingleReadAntiMessage*);
      bool Route(SingleReadMessage*);
      bool Route(SingleReadResponseMessage*);
      bool Route(StateMigrationMessage*);
      bool Route(WriteAntiMessage*);
      bool Route(WriteMessage*);
      bool Route(WriteResponseMessage*);

      unsigned int GetLpRankByDirection(Direction) const;
      Direction GetDirectionByLpRank(unsigned int) const;
      void SetSsvIdDirection(SsvId, Direction);

  };
}
#endif
