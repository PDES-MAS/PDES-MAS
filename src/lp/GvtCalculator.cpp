#include "GvtCalculator.h"
#include "Lp.h"
#include "Log.h"
#include "GvtRequestMessage.h"
#include <spdlog/spdlog.h>

GvtCalculator::GvtCalculator() {
  // Do nothing
}

GvtCalculator::GvtCalculator(Lp *pLp) :
    fLp(pLp), fColour(WHITE), fRedTime(ULONG_MAX), fIsAcceptingRequests(true) {
  fNextLpInRing = (pLp->GetRank() + 1) % pLp->GetSize();
  fIsGVTStarter = pLp->GetRank() == 0;
}

GvtCalculator::~GvtCalculator() {
  // Do nothing
}

void GvtCalculator::ProcessMessage(const GvtRequestMessage *pGvtRequestMessage) {
  // Do nothing if we're not the the GVT starter (root node)
  if (!fIsGVTStarter) return;
  // Do nothing if we're not accepting requests for GVT
  if (!fIsAcceptingRequests) return;
  //Stop accepting further GVT request messages
  fIsAcceptingRequests = false;
  // Set colour to white
  fColour = WHITE;
  // Create control message
  GvtControlMessage *gvtControlMessage = new GvtControlMessage();
  gvtControlMessage->SetOrigin(fLp->GetRank());
  gvtControlMessage->SetDestination(fNextLpInRing);
  gvtControlMessage->SetMessageMinimumTime(LONG_MAX);
  gvtControlMessage->SetRedMessageTime(LONG_MAX);
  gvtControlMessage->SetMatternCut(FIRST);
  for (unsigned int nextLp = 0; nextLp < fLp->GetSize(); ++nextLp) {
    gvtControlMessage->SetMessageCount(nextLp, 0);
  }
  // Send control message to next in the ring
  gvtControlMessage->SendToLp(fLp);
  // Original GvtRequestMessage will be deleted elsewhere
  // New GvtControlMessage will be deleted after send
}

void GvtCalculator::ProcessMessage(const GvtValueMessage *pGvtValueMessage) {
  // Do nothing if this is the GVT starter, things have already been set
  if (fIsGVTStarter) return;
  // If GVT has reached end time, barrier
  if (pGvtValueMessage->GetGVT() >= fLp->GetEndTime()) {
    spdlog::info("GvtCalculator::ProcessGvt(rank={0})# GVT value ({1}) is greater or equal to end time ({2}), barrier!",
                 fLp->GetRank(), pGvtValueMessage->GetGVT(), fLp->GetEndTime());

    MPI_Barrier(MPI_COMM_WORLD);
  }
  // Set GVT at the LP
  fLp->SetGvt(pGvtValueMessage->GetGVT());
  // Set colour to white for the next GVT check
  fColour = WHITE;
}

void GvtCalculator::ProcessMessage(const GvtControlMessage *pGvtControlMessage) {
  switch (pGvtControlMessage->GetMatternCut()) {
    case FIRST :
      ProcessFirstCutGvtControl(pGvtControlMessage);
      break;
    case SECOND :
      ProcessSecondCutGvtControl(pGvtControlMessage);
      break;
  }
}

MatternColour GvtCalculator::GetColour() const {
  return fColour;
}

void GvtCalculator::SetRedTime(unsigned long pRedTime) {
  fRedTime = pRedTime;
}

unsigned long GvtCalculator::GetRedTime() const {
  return fRedTime;
}

long GvtCalculator::GetWhiteTransientMessageCounter(unsigned int pRank) const {
  map<unsigned int, long>::const_iterator writeTransientMessageCounterIterator = fWhiteTransientMessageCounter.find(
      pRank);
  if (writeTransientMessageCounterIterator != fWhiteTransientMessageCounter.end())
    return writeTransientMessageCounterIterator->second;
  else return -1;
}

void GvtCalculator::DecrementWhiteTransientMessageCounter(unsigned int pRank) {
  fWhiteTransientMessageCounter[pRank] -= 1;
}

void GvtCalculator::IncrementWhiteTransientMessageCounter(unsigned int pRank) {
  fWhiteTransientMessageCounter[pRank] += 1;
}

void GvtCalculator::ProcessFirstCutGvtControl(
    const GvtControlMessage *pGvtControlMessage) {
  //if (msg.cut == first)
  //  add sent/received information to msg.transientVector
  //  set msg.Lvt = min(msg.Lvt, this.Lvt)
  //  if (we are the GvtStarter):
  //    if (msg.transientVector parses to 0):
  //      set this.colour = white
  //      create GvtEndMessage
  //      pass on GvtEndMessage to first in ring;
  //   else:
  //      set msg.cut = second
  //      set this.colour = red
  //      pass on message
  //  else:
  //    set this.colour = red
  //    pass on message
  // Create new GVT control message
  GvtControlMessage *newGvtControlMessage = new GvtControlMessage();
  *newGvtControlMessage = *pGvtControlMessage;
  // Set message minimum time if LP lvt is smaller
  if (fLp->GetLvt() < newGvtControlMessage->GetMessageMinimumTime())
    newGvtControlMessage->SetMessageMinimumTime(fLp->GetLvt());

  // Update transient vector information
  for (unsigned int lpCounter = 0; lpCounter < fLp->GetSize(); ++lpCounter) {
    long localCount = GetWhiteTransientMessageCounter(lpCounter);
    // Note, when lp == thisLp, localCount will be <= 0
    long total = newGvtControlMessage->GetMessageCount(lpCounter) + localCount;
    newGvtControlMessage->SetMessageCount(lpCounter, total);
  }
  // Set colour to red
  fColour = RED;
  // Set local red time to max
  fRedTime = LONG_MAX;
  // For the GVT starter
  if (fIsGVTStarter) {
    bool hasWhiteTransientsRemaining = false;
    for (map<unsigned int, long>::iterator lpIterator = fWhiteTransientMessageCounter.begin();
         lpIterator != fWhiteTransientMessageCounter.end(); ++lpIterator) {
      if (newGvtControlMessage->GetMessageCount(lpIterator->first) > 0)
        hasWhiteTransientsRemaining = true;
    }
    if (!hasWhiteTransientsRemaining) {
      // No white transients remaining, finish GVT update, just send the value
      // Set colour to white for next GVT check
      fColour = WHITE;
      // Calculate GVT, send round GVT value messages, deal with end time,
      // and set GVT at LP
      SendAndSetGvt(newGvtControlMessage);
      delete newGvtControlMessage;
    } else {
      // There are still white transient messages remaining
      // but we finished the first cut, start the second cut
      newGvtControlMessage->SetMatternCut(SECOND);
      newGvtControlMessage->SetDestination(fNextLpInRing);
      newGvtControlMessage->SendToLp(fLp);
    }
  } else {
    // We're not the GVT starter (root) node, so just continue with
    // the first cut
    newGvtControlMessage->SetDestination(fNextLpInRing);
    newGvtControlMessage->SendToLp(fLp);
  }
}

void GvtCalculator::ProcessSecondCutGvtControl(
    const GvtControlMessage *pGvtControlMessage) {
  // else if (msg.cut == second):
  //   wait until (msg.transientVector[this] == 0)
  //   set msg.Lvt = min(msg.Lvt, this.Lvt)
  //   set msg.RED = min(msg.RED, this.RED)
  //   set colour = white
  //   if (we are GvtStarter):
  //     create GvtEndMessage
  //     pass on GvtEndMessage to first in ring
  //   else:
  //     pass on message
  // Copy GVT control message
  GvtControlMessage *newGvtControlMessage = new GvtControlMessage();
  *newGvtControlMessage = *pGvtControlMessage;
  // Get number of outstanding messages
  long outstanding = newGvtControlMessage->GetMessageCount(fLp->GetRank());
  // Process the still outstanding messages
  while (outstanding + GetWhiteTransientMessageCounter(fLp->GetRank()) > 0) {
    fLp->Unlock();
    fLp->fMPIInterface->ReceiveWait();
    fLp->Lock();
    fLp->Receive();
  }
  // All outstanding messages have been processed now
  // Update message and red times
  if (fLp->GetLvt() < newGvtControlMessage->GetMessageMinimumTime())
    newGvtControlMessage->SetMessageMinimumTime(fLp->GetLvt());
  if (fRedTime < newGvtControlMessage->GetRedMessageTime())
    newGvtControlMessage->SetRedMessageTime(fRedTime);
  // Set the message count for this LP to 0
  newGvtControlMessage->SetMessageCount(fLp->GetRank(), 0);
  // Reset colour to white for next GVT check
  fColour = WHITE;
  // If we're the GVT starter
  if (fIsGVTStarter) {
    // Calculate GVT, send round GVT value messages, deal with end time, and set GVT at LP
    SendAndSetGvt(newGvtControlMessage);
    delete newGvtControlMessage;
  } else {
    // We're not the start (root) node, so continue with the second cut
    newGvtControlMessage->SetDestination(fNextLpInRing);
    newGvtControlMessage->SendToLp(fLp);
  }
}

void GvtCalculator::SendAndSetGvt(const GvtControlMessage *pGvtControlMessage) {
  // Start accepting requests
  fIsAcceptingRequests = true;
  // Calculate GVT
  unsigned long gvt;
  if (pGvtControlMessage->GetMessageMinimumTime() < pGvtControlMessage->GetRedMessageTime())
    gvt = pGvtControlMessage->GetMessageMinimumTime();
  else gvt = pGvtControlMessage->GetRedMessageTime();
  // Send round GVT value messages
  for (unsigned int lpCounter = 0; lpCounter < fLp->GetSize(); ++lpCounter) {
    if (lpCounter != fLp->GetRank()) {
      GvtValueMessage *gvtValueMessage = new GvtValueMessage();
      gvtValueMessage->SetOrigin(fLp->GetRank());
      gvtValueMessage->SetDestination(lpCounter);
      gvtValueMessage->SetGVT(gvt);
      gvtValueMessage->SendToLp(fLp);
    }
  }
  // If GVT has reached end time
  if (gvt >= fLp->GetEndTime()) {
    // Yield all control messages
    while (!fLp->fSendControlMessageQueue->IsEmpty()) {
      fLp->Unlock();
      sched_yield();
      fLp->Lock();
    }
    // And barrier
    fLp->fMPIInterface->LockMpi();
    MPI_Barrier(MPI_COMM_WORLD);
    fLp->fMPIInterface->UnlockMpi();
  }
  // Set GVT at the Lp
  fLp->SetGvt(gvt);
}
