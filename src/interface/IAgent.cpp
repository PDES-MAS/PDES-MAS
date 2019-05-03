/*
 * IAgent.cpp
 *
 *  Created on: 9 Sep 2010
 *      Author: Dr B.G.W. Craenen <b.craenen@cs.bham.ac.uk>
 */
#include <assert.h>
#include "IAgent.h"
#include "SsvId.h"
#include "SingleReadMessage.h"
#include "Helper.h"
#include "Log.h"
#include "GvtRequestMessage.h"


IAgent::IAgent(unsigned int pCommRank, unsigned int pCommSize, unsigned int pNumberOfClps, unsigned int pNumberOfAlps,
               unsigned long const &pStartTime, unsigned long const &pEndTime, const Initialisor &initialisor,
               Alp *pAlp) {
  fIdentifierHandler = new IdentifierHandler(pCommRank, pNumberOfClps, pNumberOfAlps);
  fResponseSemaphore = Semaphore();
  fPostReceiveSemaphore = Semaphore();
  fInsideMutex = Mutex(ERRORCHECK);
  fHasOutsideMessageWaiting = false;
  fHasResponseMessageWaiting = false;
  fFilePrint = FilePrint();
  fIAlp = pAlp;
}

void IAgent::SignalReceiveProcess() {
  fIAlp->SignalReceiveProcess();
}

void IAgent::SetIgnoreID() {
  fIAlp->SetIgnoreID(fIdentifierHandler->GetLastID());
}

AbstractMessage *IAgent::Read(long pAgentId, int pVariableId, unsigned long pTime) {
  LOG(logFINEST) << "IAgent::Read(" << fIAlp->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentReadLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  SingleReadMessage *singleReadMessage = new SingleReadMessage();
  singleReadMessage->SetOrigin(fIAlp->GetRank());
  singleReadMessage->SetDestination(fIAlp->GetParentClp());
  singleReadMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT calculator
  singleReadMessage->SetNumberOfHops(0);
  singleReadMessage->SetIdentifier(fIdentifierHandler->GetNextID());
  singleReadMessage->SetOriginalAlp(LpId(pAgentId, fIAlp->GetRank()));
  singleReadMessage->SetSsvId(ssvId);
  singleReadMessage->Send(fIAlp);
  SetResponseMessageWaiting(true);
  return WaitForMessage();
}

AbstractMessage *IAgent::WriteInt(long pAgentId, int pVariableId, int pIntValue, unsigned long pTime) {
  LOG(logFINEST) << "IAgent::WriteInt(" << fIAlp->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  Value<int> *value = new Value<int>(pIntValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(fIAlp->GetRank());
  writeMessage->SetDestination(fIAlp->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(fIdentifierHandler->GetNextID());
  writeMessage->SetOriginalAlp(LpId(pAgentId, fIAlp->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(fIAlp);
  SetResponseMessageWaiting(true);
  return WaitForMessage();
}

pdesmas::AbstractMessage *
IAgent::WriteDouble(long pAgentId, int pVariableId, double pDoubleValue, unsigned long pTime) {
  LOG(logFINEST) << "IAgent::WriteDouble(" << fIAlp->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  Value<double> *value = new Value<double>(pDoubleValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(fIAlp->GetRank());
  writeMessage->SetDestination(fIAlp->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(fIdentifierHandler->GetNextID());
  writeMessage->SetOriginalAlp(LpId(pAgentId, fIAlp->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(fIAlp);
  SetResponseMessageWaiting(true);
  return WaitForMessage();
}

pdesmas::AbstractMessage *
IAgent::WritePoint(long pAgentId, int pVariableId, const Point pPairValue, unsigned long pTime) {
  LOG(logFINEST) << "IAgent::WritePoint(" << fIAlp->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  Value<Point> *value = new Value<Point>(pPairValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(fIAlp->GetRank());
  writeMessage->SetDestination(fIAlp->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(fIdentifierHandler->GetNextID());
  writeMessage->SetOriginalAlp(LpId(pAgentId, fIAlp->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(fIAlp);
  SetResponseMessageWaiting(true);
  return WaitForMessage();
}

pdesmas::AbstractMessage *
IAgent::WriteString(long pAgentId, int pVariableId, const string pStringValue, unsigned long pTime) {
  LOG(logFINEST) << "IAgent::WriteString(" << fIAlp->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  // TODO: LVT update in ALP
  SsvId ssvId(pVariableId);
  Value<string> *value = new Value<string>(pStringValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(fIAlp->GetRank());
  writeMessage->SetDestination(fIAlp->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(fIdentifierHandler->GetNextID());
  writeMessage->SetOriginalAlp(LpId(pAgentId, fIAlp->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(fIAlp);
  SetResponseMessageWaiting(true);
  return WaitForMessage();
}

pdesmas::AbstractMessage *IAgent::RangeQuery(long pAgentId, unsigned long pTime, Point pStartValue, Point pEndValue) {
  LOG(logFINEST) << "IAgent::RangeQuery(" << fIAlp->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentReadLVT(pAgentId, pTime);
  Range range(pStartValue, pEndValue);
  RangeQueryMessage *rangeQueryMessage = new RangeQueryMessage();
  rangeQueryMessage->SetOrigin(fIAlp->GetRank());
  rangeQueryMessage->SetDestination(fIAlp->GetParentClp());
  rangeQueryMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  rangeQueryMessage->SetNumberOfHops(0);
  rangeQueryMessage->SetIdentifier(fIdentifierHandler->GetNextID());
  rangeQueryMessage->SetOriginalAlp(LpId(pAgentId, fIAlp->GetRank()));
  rangeQueryMessage->SetRange(range);
  // Valuemap?
  rangeQueryMessage->SetNumberOfTraverseHops(0);
  rangeQueryMessage->Send(fIAlp);
  SetResponseMessageWaiting(true);
  return WaitForMessage();
}

void IAgent::SendGVTMessage() {
  GvtRequestMessage *gvtMessage = new GvtRequestMessage();
  gvtMessage->SetOrigin(fIAlp->GetRank());
  gvtMessage->SetDestination(0);
  gvtMessage->Send(fIAlp);
  Unlock();
}

void IAgent::SendEndMessage() {
  EndMessage *endMessage = new EndMessage();
  endMessage->SetOrigin(fIAlp->GetRank());
  endMessage->SetDestination(fIAlp->GetParentClp());
  endMessage->SetSenderAlp(fIAlp->GetRank());
  endMessage->Send(fIAlp);
  SetResponseMessageWaiting(true);
  // endMessage will be deleted after it has been send
  fResponseSemaphore.Wait();
}

AbstractMessage *IAgent::WaitForMessage() {
  fIAlp->SignalPreResponse();
  Unlock();
  fResponseSemaphore.Wait();
  Lock();
  return GetResponseMessage();
}

AbstractMessage *IAgent::GetOutsideMessage() const {
  return GetResponseMessage();
}

void IAgent::SignalOutsideRollback() {
  fIAlp->SignalOutsideMessage();
}

bool IAgent::SignalResponse(unsigned long pMessageId, long pAgentId) {
  if (pMessageId != fIdentifierHandler->GetLastID()) {
    LOG(logWARNING) << "IAgent::SignalResponse(double,long)(" << fIAlp->GetRank()
                    << ")# Incorrect message ID for signal response: " << pMessageId << ", instead of: "
                    << fIdentifierHandler->GetLastID();
    return false;
  }
//  if (pAgentId != GetLastLVTAgentID()) {
//    LOG(logWARNING) << "IAgent::SignalResponse(double,long)(" << fIAlp->GetRank()
//                    << ")# Incorrect agent ID for signal response: " << pAgentId << ", instead of: "
//                    << GetLastLVTAgentID();
//    return false;
//  }
  fResponseSemaphore.Signal();
  return true;
}

void IAgent::SignalResponse() {
  fResponseSemaphore.Signal();
}

void IAgent::Lock() {
  fInsideMutex.Lock();
}

void IAgent::Unlock() {
  fInsideMutex.Unlock();
}

unsigned long IAgent::GetGvt() const {
  return fIAlp->GetGvt();
}

void IAgent::SignalPostReceive() {
  fPostReceiveSemaphore.Signal();
}

void IAgent::PostReceiveWait() {
  fPostReceiveSemaphore.Wait();
}

void IAgent::SignalPostOutside() {
  fPostOutsideSemaphore.Signal();
}

void IAgent::PostOutsideWait() {
  fPostOutsideSemaphore.Wait();
}

bool IAgent::HasResponseMessageWaiting() const {
  return fHasResponseMessageWaiting;
}

bool IAgent::HasOutsideMessageWaiting() const {
  return fHasOutsideMessageWaiting;
}

void IAgent::SetResponseMessageWaiting(bool pHasResponseMessageWaiting) {
  fHasResponseMessageWaiting = pHasResponseMessageWaiting;
}

void IAgent::SetOutsideMessageWaiting(bool pHasOutsideMessageWaiting) {
  fHasOutsideMessageWaiting = pHasOutsideMessageWaiting;
}
