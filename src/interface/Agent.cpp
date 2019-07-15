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


Agent::Agent(unsigned long const start_time, unsigned long const end_time, unsigned long agent_id) :
    start_time_(start_time), end_time_(end_time), agent_id_(agent_id) {
}


void *Agent::MyThread(void *arg) {

  spdlog::debug("Agent thread is up");

  while (GetLVT() < end_time_) {
    Cycle();
  }
  spdlog::debug("GVT >= EndTime, agent exit, id={0}", this->get_id());
  SendGVTMessage(); // Initiate GVT calculation to get ready for termination
  this->Stop();
  return nullptr;
}

const SingleReadResponseMessage *Agent::SendReadMessageAndGetResponse(unsigned long pVariableId, unsigned long pTime) {
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
  const AbstractMessage *ret = attached_alp_->GetResponseMessage(agent_identifier_.GetId());
  return dynamic_cast<const SingleReadResponseMessage *>(ret);
}

template<typename T>
const WriteResponseMessage *
Agent::SendWriteMessageAndGetResponse(unsigned long pVariableId, T pValue, unsigned long pTime) {
  LOG(logFINEST) << "Agent::WriteInt(" << attached_alp_->GetRank() << ")# Set LVT to: " << pTime;
  //SetAgentWriteLVT(pAgentId, pTime);
  SsvId ssvId(pVariableId);
  Value<T> *value = new Value<T>(pValue);
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
  const AbstractMessage *ret = attached_alp_->GetResponseMessage(agent_identifier_.GetId());
  return dynamic_cast<const WriteResponseMessage *>(ret);

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

unsigned long Agent::GetAlpLVT() const {
  return attached_alp_->GetLvt();
}

void Agent::time_wrap(unsigned long t) {
  this->attached_alp_->SetAgentLvt(this->agent_identifier_.GetId(),
                                   this->attached_alp_->GetAgentLvt(this->agent_identifier_.GetId()) + t);
}

void Agent::attach_alp(Alp *alp) {
  if (alp != nullptr) {
    this->attached_alp_ = alp;
    agent_identifier_ = LpId(agent_id_, attached_alp_->GetRank());
  } else {
    spdlog::error("Attached ALP is nullptr!");
  }
}

const int Agent::ReadInt(unsigned long variable_id, unsigned long timestamp) {
  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
  return dynamic_cast<const Value<int> *>(ret->GetValue())->GetValue();
}


const double Agent::ReadDouble(unsigned long variable_id, unsigned long timestamp) {
  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
  return dynamic_cast<const Value<double> *>(ret->GetValue())->GetValue();
}

const Point Agent::ReadPoint(unsigned long variable_id, unsigned long timestamp) {
  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
  return dynamic_cast<const Value<Point> *>(ret->GetValue())->GetValue();
}

const string Agent::ReadString(unsigned long variable_id, unsigned long timestamp) {
  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
  return dynamic_cast<const Value<string> *>(ret->GetValue())->GetValue();
}

const int Agent::ReadPrivateInt(unsigned long variable_id) {
  return 0;
}

const double Agent::ReadPrivateDouble(unsigned long variable_id) {
  return 0;
}

const Point Agent::ReadPrivatePoint(unsigned long variable_id) {
  return Point();
}

const string Agent::ReadPrivateString(unsigned long variable_id) {
  return std::__cxx11::string();
}

bool Agent::WritePrivateInt(unsigned long variable_id) {
  return false;
}

bool Agent::WritePrivateDouble(unsigned long variable_id) {
  return false;
}

bool Agent::WritePrivatePoint(unsigned long variable_id) {
  return false;
}

bool Agent::WritePrivateString(unsigned long variable_id) {
  return false;
}

bool Agent::WriteInt(unsigned long variable_id, int value, unsigned long timestamp) {
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<int>(variable_id, value, timestamp);

  WriteStatus status = ret->GetWriteStatus();
  return status == writeSUCCESS;
}

bool Agent::WriteDouble(unsigned long variable_id, double value, unsigned long timestamp) {
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<double>(variable_id, value, timestamp);

  WriteStatus status = ret->GetWriteStatus();
  return status == writeSUCCESS;
}

bool Agent::WritePoint(unsigned long variable_id, Point value, unsigned long timestamp) {
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<Point>(variable_id, value, timestamp);

  WriteStatus status = ret->GetWriteStatus();
  return status == writeSUCCESS;
}

bool Agent::WriteString(unsigned long variable_id, string value, unsigned long timestamp) {
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<string>(variable_id, value, timestamp);

  WriteStatus status = ret->GetWriteStatus();
  return status == writeSUCCESS;
}


const SerialisableMap<SsvId, Value<Point> >
Agent::RangeQueryPoint(const Point start, const Point end, unsigned long timestamp) {
  return SerialisableMap<SsvId, Value<Point>>();
}


