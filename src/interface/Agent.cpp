/*
 * Agent.h
 *
 *  Created on: 4 May 2019
 *      Author: CubicPill
 */

#include <assert.h>
#include "Agent.h"
#include "SsvId.h"
#include "SingleReadMessage.h"
#include "Helper.h"
#include "Log.h"
#include "GvtRequestMessage.h"


Agent::Agent(unsigned long const &start_time, unsigned long const &end_time, Alp *parent_alp, unsigned long agent_id) {
  attached_alp_ = parent_alp;
  identifier_ = LpId(agent_id, attached_alp_->GetRank());
}


const AbstractMessage *Agent::Read(long pAgentId, int pVariableId, unsigned long pTime) {
  LOG(logFINEST) << "Agent::Read(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentReadLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  SingleReadMessage *singleReadMessage = new SingleReadMessage();
  singleReadMessage->SetOrigin(attached_alp_->GetRank());
  singleReadMessage->SetDestination(attached_alp_->GetParentClp());
  singleReadMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT calculator
  singleReadMessage->SetNumberOfHops(0);
  singleReadMessage->SetIdentifier(attached_alp_->GetNewMessageId());
  singleReadMessage->SetOriginalAgent(LpId(pAgentId, attached_alp_->GetRank()));
  singleReadMessage->SetSsvId(ssvId);
  singleReadMessage->Send(attached_alp_);
  WaitForMessage();
  return attached_alp_->GetResponseMessage(pAgentId);
}

const AbstractMessage *Agent::WriteInt(long pAgentId, int pVariableId, int pIntValue, unsigned long pTime) {
  LOG(logFINEST) << "Agent::WriteInt(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  Value<int> *value = new Value<int>(pIntValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(attached_alp_->GetRank());
  writeMessage->SetDestination(attached_alp_->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(attached_alp_->GetNewMessageId());
  writeMessage->SetOriginalAgent(LpId(pAgentId, attached_alp_->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(attached_alp_);
  WaitForMessage();
  return attached_alp_->GetResponseMessage(pAgentId);
}

const pdesmas::AbstractMessage *
Agent::WriteDouble(long pAgentId, int pVariableId, double pDoubleValue, unsigned long pTime) {
  LOG(logFINEST) << "Agent::WriteDouble(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  Value<double> *value = new Value<double>(pDoubleValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(attached_alp_->GetRank());
  writeMessage->SetDestination(attached_alp_->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(attached_alp_->GetNewMessageId());
  writeMessage->SetOriginalAgent(LpId(pAgentId, attached_alp_->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(attached_alp_);
  WaitForMessage();
  return attached_alp_->GetResponseMessage(pAgentId);
}

const pdesmas::AbstractMessage *
Agent::WritePoint(long pAgentId, int pVariableId, const Point pPairValue, unsigned long pTime) {
  LOG(logFINEST) << "Agent::WritePoint(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  Value<Point> *value = new Value<Point>(pPairValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(attached_alp_->GetRank());
  writeMessage->SetDestination(attached_alp_->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(attached_alp_->GetNewMessageId());
  writeMessage->SetOriginalAgent(LpId(pAgentId, attached_alp_->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(attached_alp_);
  WaitForMessage();
  return attached_alp_->GetResponseMessage(pAgentId);
}

const pdesmas::AbstractMessage *
Agent::WriteString(long pAgentId, int pVariableId, const string pStringValue, unsigned long pTime) {
  LOG(logFINEST) << "Agent::WriteString(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  // TODO: LVT update in ALP
  SsvId ssvId(pVariableId);
  Value<string> *value = new Value<string>(pStringValue);
  WriteMessage *writeMessage = new WriteMessage();
  writeMessage->SetOrigin(attached_alp_->GetRank());
  writeMessage->SetDestination(attached_alp_->GetParentClp());
  writeMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  writeMessage->SetNumberOfHops(0);
  writeMessage->SetIdentifier(attached_alp_->GetNewMessageId());
  writeMessage->SetOriginalAgent(LpId(pAgentId, attached_alp_->GetRank()));
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->Send(attached_alp_);
  WaitForMessage();
  return attached_alp_->GetResponseMessage(pAgentId);
}

const pdesmas::AbstractMessage *
Agent::RangeQuery(long pAgentId, unsigned long pTime, Point pStartValue, Point pEndValue) {
  LOG(logFINEST) << "Agent::RangeQuery(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentReadLVT(pAgentId, pTime);
  Range range(pStartValue, pEndValue);
  RangeQueryMessage *rangeQueryMessage = new RangeQueryMessage();
  rangeQueryMessage->SetOrigin(attached_alp_->GetRank());
  rangeQueryMessage->SetDestination(attached_alp_->GetParentClp());
  rangeQueryMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT Calculator
  rangeQueryMessage->SetNumberOfHops(0);
  rangeQueryMessage->SetIdentifier(attached_alp_->GetNewMessageId());
  rangeQueryMessage->SetOriginalAgent(LpId(pAgentId, attached_alp_->GetRank()));
  rangeQueryMessage->SetRange(range);
  // Valuemap?
  rangeQueryMessage->SetNumberOfTraverseHops(0);
  rangeQueryMessage->Send(attached_alp_);
  WaitForMessage();
  return attached_alp_->GetResponseMessage(pAgentId);
}

void Agent::SendGVTMessage() {
  GvtRequestMessage *gvtMessage = new GvtRequestMessage();
  gvtMessage->SetOrigin(attached_alp_->GetRank());
  gvtMessage->SetDestination(0);
  gvtMessage->Send(attached_alp_);
}

void Agent::SendEndMessage() {
  EndMessage *endMessage = new EndMessage();
  endMessage->SetOrigin(attached_alp_->GetRank());
  endMessage->SetDestination(attached_alp_->GetParentClp());
  endMessage->SetSenderAlp(attached_alp_->GetRank());
  endMessage->Send(attached_alp_);
}

void Agent::WaitForMessage() {
  attached_alp_->WaitForResponseMessageToArrive(semaphore_has_response_);
  semaphore_has_response_.Wait();


}

void Agent::Finalise() {

}

