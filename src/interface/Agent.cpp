/*
 * Agent.h
 *
 *  Created on: 4 May 2019
 *      Author: CubicPill
 *  The agent class itself should be completely stateless, all stateful information should be stored
 *  in private storage provided by ALP
 */

#include <assert.h>
#include "Agent.h"
#include "SsvId.h"
#include "SingleReadMessage.h"
#include "Helper.h"
#include "Log.h"
#include "GvtRequestMessage.h"
#include "spdlog/spdlog.h"


Agent::Agent(unsigned long const start_time, unsigned long const end_time, Alp *parent_alp, unsigned long agent_id) :
    attached_alp_(parent_alp), start_time_(start_time), end_time_(end_time) {
  if (attached_alp_ == nullptr) {
    spdlog::error("Parent ALP is nullptr!");
    exit(1);
  }
  agent_identifier_ = LpId(agent_id, attached_alp_->GetRank());
}


void *Agent::MyThread(void *arg) {
  spdlog::debug("Agent thread is up");


  while (GetLVT() < end_time_) {
    Cycle();
  }

  pthread_exit(nullptr);
}

const AbstractMessage *Agent::SendReadMessageAndGetResponse(unsigned long pVariableId, unsigned long pTime) {
  LOG(logFINEST) << "Agent::SendReadMessageAndGetResponse(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentReadLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  SingleReadMessage *singleReadMessage = new SingleReadMessage();
  singleReadMessage->SetOrigin(attached_alp_->GetRank());
  singleReadMessage->SetDestination(attached_alp_->GetParentClp());
  singleReadMessage->SetTimestamp(pTime);
  // Mattern colour set by GVT calculator
  singleReadMessage->SetNumberOfHops(0);
  singleReadMessage->SetIdentifier(attached_alp_->GetNewMessageId());
  singleReadMessage->SetOriginalAgent(agent_identifier_);
  singleReadMessage->SetSsvId(ssvId);
  singleReadMessage->SendToLp(attached_alp_);
  WaitUntilMessageArrive();
  return attached_alp_->GetResponseMessage(agent_identifier_.GetId());
}

const AbstractMessage *
Agent::SendWriteIntMessageAndGetResponse(unsigned long pVariableId, int pIntValue, unsigned long pTime) {
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
  writeMessage->SetOriginalAgent(agent_identifier_);
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->SendToLp(attached_alp_);
  WaitUntilMessageArrive();
  return attached_alp_->GetResponseMessage(agent_identifier_.GetId());
}

const pdesmas::AbstractMessage *
Agent::SendWriteDoubleMessageAndGetResponse(unsigned long pVariableId, double pDoubleValue, unsigned long pTime) {
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
  writeMessage->SetOriginalAgent(agent_identifier_);
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->SendToLp(attached_alp_);
  WaitUntilMessageArrive();
  return attached_alp_->GetResponseMessage(agent_identifier_.GetId());
}

const pdesmas::AbstractMessage *
Agent::SendWritePointMessageAndGetResponse(unsigned long pVariableId, const Point pPairValue, unsigned long pTime) {
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
  writeMessage->SetOriginalAgent(agent_identifier_);
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->SendToLp(attached_alp_);
  WaitUntilMessageArrive();
  return attached_alp_->GetResponseMessage(agent_identifier_.GetId());
}

const pdesmas::AbstractMessage *
Agent::SendWriteStringMessageAndGetResponse(unsigned long pVariableId, const string pStringValue, unsigned long pTime) {
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
  writeMessage->SetOriginalAgent(agent_identifier_);
  writeMessage->SetSsvId(ssvId);
  writeMessage->SetValue(value);
  writeMessage->SendToLp(attached_alp_);
  WaitUntilMessageArrive();
  return attached_alp_->GetResponseMessage(agent_identifier_.GetId());
}

const pdesmas::AbstractMessage *
Agent::SendRangeQueryPointMessageAndGetResponse(unsigned long pTime, const Point pStartValue, const Point pEndValue) {
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
  rangeQueryMessage->SetOriginalAgent(agent_identifier_);
  rangeQueryMessage->SetRange(range);
  // Valuemap?
  rangeQueryMessage->SetNumberOfTraverseHops(0);
  rangeQueryMessage->SendToLp(attached_alp_);
  WaitUntilMessageArrive();
  return attached_alp_->GetResponseMessage(agent_identifier_.GetId());
}

void Agent::SendGVTMessage() {
  GvtRequestMessage *gvtMessage = new GvtRequestMessage();
  gvtMessage->SetOrigin(attached_alp_->GetRank());
  gvtMessage->SetDestination(0);
  gvtMessage->SendToLp(attached_alp_);
}

void Agent::SendEndMessage() {
  EndMessage *endMessage = new EndMessage();
  endMessage->SetOrigin(attached_alp_->GetRank());
  endMessage->SetDestination(attached_alp_->GetParentClp());
  endMessage->SetSenderAlp(attached_alp_->GetRank());
  endMessage->SendToLp(attached_alp_);
}

void Agent::WaitUntilMessageArrive() {
  Semaphore &semaphore_has_response_ = attached_alp_->GetWaitingSemaphore(agent_identifier_.GetId());
  semaphore_has_response_.Wait();
}

void Agent::Finalise() {

}

bool Agent::SetLVT(unsigned long lvt) {
  return attached_alp_->SetAgentLvt(agent_identifier_.GetId(), lvt);
}

unsigned long Agent::GetLVT() const {
  return attached_alp_->GetAgentLvt(agent_identifier_.GetId());
}


unsigned long Agent::GetGVT() const {
  return attached_alp_->GetGvt();
}

void Agent::time_wrap(unsigned long t) {
  this->attached_alp_->SetAgentLvt(this->agent_identifier_.GetId(),
                                   this->attached_alp_->GetAgentLvt(this->agent_identifier_.GetId()) + t);
}