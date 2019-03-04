#include "Router.h"
#include "SingleReadAntiMessage.h"
#include "WriteAntiMessage.h"

using namespace pdesmas;

Router::Router() {
  // Nothing to do
}

Router::Router(unsigned int pLpRank, unsigned int pNumberOfClps, const Initialisor* pInitialisor) : fLpRank(pLpRank) {
  fRouteTable = RouteTable(pLpRank, pNumberOfClps, pInitialisor);
}

Router::~Router() {
  // Nothing to do
}

bool Router::Route(RollbackMessage* pRollbackMessage) {
  Direction direction = fRouteTable.GetDirectionFromRank(pRollbackMessage->GetOriginalAlp().GetRank());
  if (direction == HERE) return true;
  pRollbackMessage->SetDestination(fRouteTable.GetRankFromDirection(direction));
  return false;
}

bool Router::Route(SingleReadAntiMessage* pSingleReadAntiMessage) {
  Direction direction = fRouteTable.GetDirectionFromSsvId(pSingleReadAntiMessage->GetSsvId());
  if (direction == DIRECTION_SIZE) {
    LOG(logERROR) << "Router::Route(SingleReadAntiMessage)# Could not find direction from SSVID for SingleReadAntiMessage: " << *pSingleReadAntiMessage;
    exit(1);
  }
  if (direction == HERE) return true;
  pSingleReadAntiMessage->SetDestination(fRouteTable.GetRankFromDirection(direction));
  return false;
}

bool Router::Route(SingleReadMessage* pSingleReadMessage) {
  Direction direction = fRouteTable.GetDirectionFromSsvId(pSingleReadMessage->GetSsvId());
  if (direction == DIRECTION_SIZE) {
    LOG(logERROR) << "Router::Route(SingleReadMessage)# Could not find direction from SSVID for SingleReadAntiMessage: " << *pSingleReadMessage;
    exit(1);
  }
  if (direction == HERE) return true;
  pSingleReadMessage->SetDestination(fRouteTable.GetRankFromDirection(direction));
  return false;
}

bool Router::Route(SingleReadResponseMessage* pSingleReadResponseMessage) {
  Direction direction = fRouteTable.GetDirectionFromRank(pSingleReadResponseMessage->GetOriginalAlp().GetRank());
  if (direction == HERE) return true;
  pSingleReadResponseMessage->SetDestination(fRouteTable.GetRankFromDirection(direction));
  return false;
}

bool Router::Route(StateMigrationMessage* pStateMigrationMessage) {
  if (pStateMigrationMessage->GetDestination() == fLpRank) return true;
  return false;
}

bool Router::Route(WriteAntiMessage* pWriteAntiMessage) {
  Direction direction = fRouteTable.GetDirectionFromSsvId(pWriteAntiMessage->GetSsvId());
  if (direction == DIRECTION_SIZE) {
    LOG(logERROR) << "Router::Route(WriteAntiMessage)# Could not find direction from SSVID for SingleReadAntiMessage: " << *pWriteAntiMessage;
    exit(1);
  }
  if (direction == HERE) return true;
  pWriteAntiMessage->SetDestination(fRouteTable.GetRankFromDirection(direction));
  return false;

}

bool Router::Route(WriteMessage* pWriteMessage) {
  Direction direction = fRouteTable.GetDirectionFromSsvId(pWriteMessage->GetSsvId());
  if (direction == DIRECTION_SIZE) {
    LOG(logERROR) << "Router::Route(WriteMessage)# Could not find direction from SSVID for SingleReadAntiMessage: " << *pWriteMessage;
    exit(1);
  }
  if (direction == HERE) return true;
  pWriteMessage->SetDestination(fRouteTable.GetRankFromDirection(direction));
  return false;
}

bool Router::Route(WriteResponseMessage* pWriteResponseMessage) {
  Direction direction = fRouteTable.GetDirectionFromRank(pWriteResponseMessage->GetOriginalAlp().GetRank());
  if (direction == HERE) return true;
  pWriteResponseMessage->SetDestination(fRouteTable.GetRankFromDirection(direction));
  return false;
}

unsigned int Router::GetLpRankByDirection(Direction pDirection) const {
  return fRouteTable.GetRankFromDirection(pDirection);
}

Direction Router::GetDirectionByLpRank(unsigned int pLpRank) const {
  return fRouteTable.GetDirectionFromRank(pLpRank);
}

void Router::SetSsvIdDirection(SsvId SsvID, Direction pDirection) {
  fRouteTable.SetSsvIdHost(SsvID, pDirection);
}
