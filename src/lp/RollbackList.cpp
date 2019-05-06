using namespace std;

#include <map>
#include "RollbackList.h"
#include "SharedState.h"
#include "RollbackMessage.h"
#include "Lp.h"
#include "Log.h"

using namespace pdesmas;
/*************************************
 METHOD: addLp

 DESC: Called when an LP has been identified as needing a rollback. The
 functions checks to see if lp is already in the rollbacklist, if so
 the time of rollback is updated if t is less than the time currently
 stored.

 ARGS: LpId& lp - the lp to be rolledback, unsigned long& t - time to rollback to
 RETURN: void
 **************************************/
using namespace pdesmas;

void RollbackList::AddLp(LpId const& lp, unsigned long t) {

  if (list.count(lp)) {//lp already in list
    if (t < list[lp]) list[lp] = t;
  } else {//Not already in list
    list[lp] = t;
  }
}

/*************************************
 METHOD: SendRollbacks

 DESC: Takes all the rollbacks currently in this rollback list and
 sends rollback messages to relevants Lps from the passed lp.

 RETURN: void
 **************************************/
void RollbackList::SendRollbacks(Lp* lp, const RollbackTag& rbTag) {
  map<LpId, unsigned long>::iterator l = list.begin();

  while (l != list.end()) {
    RollbackMessage* rollbackMessage = new RollbackMessage();
    rollbackMessage->SetOrigin(lp->GetRank());
    rollbackMessage->SetDestination(l->first.GetRank());
    // Rollback message timestamp is the time of the read to be rolled back
    rollbackMessage->SetTimestamp(l->second);
    // Mattern colour set by GVT Calculator
    rollbackMessage->SetRollbackTag(rbTag);
    rollbackMessage->SetOriginalAgent(l->first);
    rollbackMessage->SendToLp(lp);
    //Next rb in rblist.
    l++;
  }

  //After we've processed all rollbacks we clear the list of requests
  //ready for next time
  list.clear();
}

void RollbackList::SendRollbacksAfterStateMigration(Lp* lp, const RollbackTag& rbTag) {
  map<LpId, unsigned long>::iterator l = list.begin();

  while (l != list.end()) {
    RollbackMessage* rollbackMessage = new RollbackMessage();
    rollbackMessage->SetOrigin(lp->GetRank());
    rollbackMessage->SetDestination(l->first.GetRank());
    // Rollback message timestamp is the time of the read to be rolled back
    rollbackMessage->SetTimestamp(l->second + 1);
    // Mattern colour set by GVT Calculator
    rollbackMessage->SetRollbackTag(rbTag);
    rollbackMessage->SetOriginalAgent(l->first);
    rollbackMessage->SendToLp(lp);
    //Next rb in rblist.
    l++;
  }

  //After we've processed all rollbacks we clear the list of requests
  //ready for next time
  list.clear();
}
