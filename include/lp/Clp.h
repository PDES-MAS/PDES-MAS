#ifndef _CLP_H_
#define _CLP_H_

#include <string>
#include "Lp.h"
#include "SharedState.h"
#include "RollbackList.h"
#include "WriteMessage.h"
#include "SingleReadMessage.h"
#include "SsvId.h"
#include "LpId.h"
#include "AbstractValue.h"
#include "AccessCostCalculator.h"
#include "RangeTracker.h"
#include "RangeQueryMessage.h"
#include "RangeRoutingTable.h"
#include "RangeUpdateMessage.h"
#include "EndMessage.h"

using namespace std;

namespace pdesmas {
#define ROLLBACK_BY_WRITE 1
#define ROLLBACK_BY_RU 2
#define ROLLBACK_BY_SM 3

  class Clp: public Lp {
    private:
      Router* fRouter;
      SharedState fSharedState;
      bool fEndMessageProcessed;

      unsigned long GetLvt() const;

      void ProcessMessage(const SingleReadMessage*);
      void ProcessMessage(const SingleReadAntiMessage*);
      void ProcessMessage(const WriteMessage*);
      void ProcessMessage(const WriteAntiMessage*);
      void ProcessMessage(const EndMessage*);
      void PostProcessMessage();

      void Initialise();
      void Finalise();

#ifdef SSV_LOCALISATION
      AccessCostCalculator* fAccessCostCalculator;
      bool fStopLoadBalanceProcessing;
      void ProcessMessage(const StateMigrationMessage*);
      void MigrateStateVariables(const map<Direction, list<SsvId> >&);
#endif
#ifdef RANGE_QUERIES
      vector<RangeRoutingTable*> fRangeRoutingTable;
      RangeTracker* fRangeTracker;
      void ProcessMessage(const RangeQueryMessage*);
      void ProcessMessage(const RangeQueryAntiMessage*);
      void ProcessMessage(const RangeUpdateMessage*);
      void SendRangeUpdates(const list<RangeUpdates>&, Direction);
      void InitialisePortRanges(const Initialisor*);
#endif

    public:
      Clp();
      Clp(unsigned int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long, const string);
      ~Clp();

      void AddSSV(const SsvId&, const AbstractValue*);
      void SetGvt(unsigned long);
      void Send();
      void Receive();
  };
}
#endif
