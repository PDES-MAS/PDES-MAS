/*
 * Agent.h
 *
 *  Created on: 4 May 2019
 *      Author: CubicPill
 *  The agent class itself should be completely stateless, all stateful information should be stored
 *  in private storage provided by ALP
 */

#include <assert.h>
#include <sstream>
#include "Agent.h"
#include "SsvId.h"
#include "SingleReadMessage.h"
#include "Helper.h"
#include "Log.h"
#include "GvtRequestMessage.h"
#include "spdlog/spdlog.h"
#include "Types.h"


Agent::Agent(unsigned long const start_time, unsigned long const end_time, unsigned long agent_id) :
    start_time_(start_time), end_time_(end_time), agent_id_(agent_id) {
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, nullptr);
}


void Agent::Body() {

  //spdlog::debug("Agent thread is up");

  while (GetLVT() < end_time_) {
    Cycle();
  }
  spdlog::debug("Agent {0} exit, LVT {1}, GVT {2}", this->agent_id(), this->GetLVT(), this->GetGVT());
  spdlog::debug("LVT >= EndTime, agent exit, id={0}", this->agent_id());

  while (this->GetGVT() < end_time_) {
    usleep(1000 * 1000);
    SendGVTMessage(); // Initiate GVT calculation to get ready for termination
    spdlog::info("Agent {} finsihed, GVT {}, LVT {}, ALP LVT {}", this->agent_id(), this->GetGVT(), this->GetLVT(),
                 this->GetAlpLVT());
  }


}

const SingleReadResponseMessage *Agent::SendReadMessageAndGetResponse(unsigned long pVariableId, unsigned long pTime) {
  //spdlog::debug("Agent {} SendReadMessageAndGetResponse", this->agent_id());
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
  if (ret->GetType() != SINGLEREADRESPONSEMESSAGE) {
    spdlog::error("Expecting SINGLEREADRESPONSEMESSAGE, get {}", ret->GetType());
    exit(1);
  }
  //spdlog::debug("Message get, agent {0}",agent_id_);
  return (const SingleReadResponseMessage *) ret;
}

template<typename T>
const WriteResponseMessage *
Agent::SendWriteMessageAndGetResponse(unsigned long pVariableId, T pValue, unsigned long pTime) {
  //spdlog::debug("Agent {} SendWriteMessageAndGetResponse", this->agent_id());
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
  assert(ret->GetType() == WRITERESPONSEMESSAGE);
  return (const WriteResponseMessage *) ret;

}


const RangeQueryMessage *
Agent::SendRangeQueryPointMessageAndGetResponse(unsigned long pTime, const Point pStartValue, const Point pEndValue) {
  //spdlog::debug("Agent {} SendRangeQueryPointMessageAndGetResponse", this->agent_id());
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
  const AbstractMessage *ret = attached_alp_->GetResponseMessage(agent_identifier_.GetId());
  return (const RangeQueryMessage *) ret;
}

void Agent::SendGVTMessage() {
  GvtRequestMessage *gvtMessage = new GvtRequestMessage();
  gvtMessage->SetOrigin(attached_alp_->GetRank());
  gvtMessage->SetDestination(0);
  gvtMessage->SendToLp(attached_alp_);
}


void Agent::WaitUntilMessageArrive() {
//  if (attached_alp_->GetCancelFlag(this->agent_id_)) {
//    while (1) {
//      this->Sleep(1000);
//    }
//  }
  //Semaphore &semaphore_has_response_ = attached_alp_->GetWaitingSemaphore(agent_identifier_.GetId());
  //spdlog::debug("Waiting... agent {0}",this->agent_id());
  this->message_waiting_sem_.Wait();
  //semaphore_has_response_.Wait();
  //spdlog::debug("Wait finished! agent {0}",this->agent_id());
  if (attached_alp_->GetCancelFlag(this->agent_id_)) {

    this->SyncPoint();
    spdlog::error("Not correctly terminated with cancel flag set!");
    exit(1);
  }
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
  this->SetLVT(this->GetLVT() + t);

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
  assert(timestamp > this->GetLVT());
  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
  auto v = dynamic_cast<const Value<int> *>(ret->GetValue())->GetValue();
  this->SetLVT(timestamp);
  return v;

}


const double Agent::ReadDouble(unsigned long variable_id, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());

  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
  auto v = ((const Value<double> *) (ret->GetValue()))->GetValue();
  this->SetLVT(timestamp);
  return v;
}

const Point Agent::ReadPoint(unsigned long variable_id, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());
  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
//  ostringstream out=ostringstream();
//  ret->Serialise(out);
//  spdlog::debug(out.str());
  assert(ret != nullptr);
  assert(ret->GetValue() != nullptr);

  Value<Point> *p = new Value<Point>();
//spdlog::debug(p->GetValueString());
  auto p_v = ret->GetValue();

  auto s_v = p_v->GetValueString();
  p->SetValue(s_v);
  //auto v = (dynamic_cast<const Value<Point> *>(ret->GetValue()))->GetValue();
  auto v = p->GetValue();

  this->SetLVT(timestamp);
  return v;
}

const string Agent::ReadString(unsigned long variable_id, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());
  const SingleReadResponseMessage *ret = this->SendReadMessageAndGetResponse(variable_id, timestamp);
  auto v = dynamic_cast<const Value<string> *>(ret->GetValue())->GetValue();
  this->SetLVT(timestamp);
  return v;
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

bool Agent::WritePrivateInt(unsigned long variable_id, int v) {
  return false;
}

bool Agent::WritePrivateDouble(unsigned long variable_id, double v) {
  return false;
}

bool Agent::WritePrivatePoint(unsigned long variable_id, Point v) {
  return false;
}

bool Agent::WritePrivateString(unsigned long variable_id, string v) {
  return false;
}

bool Agent::WriteInt(unsigned long variable_id, int value, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<int>(variable_id, value, timestamp);

  WriteStatus status = ret->GetWriteStatus();
  this->SetLVT(timestamp);
  return status == writeSUCCESS;
}

bool Agent::WriteDouble(unsigned long variable_id, double value, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<double>(variable_id, value, timestamp);
  assert(ret != nullptr);
  WriteStatus status = ret->GetWriteStatus();
  this->SetLVT(timestamp);

  return status == writeSUCCESS;
}

bool Agent::WritePoint(unsigned long variable_id, Point value, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<Point>(variable_id, value, timestamp);

  WriteStatus status = ret->GetWriteStatus();
  this->SetLVT(timestamp);

  return status == writeSUCCESS;
}

bool Agent::WriteString(unsigned long variable_id, string value, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());
  const WriteResponseMessage *ret = SendWriteMessageAndGetResponse<string>(variable_id, value, timestamp);

  WriteStatus status = ret->GetWriteStatus();
  this->SetLVT(timestamp);

  return status == writeSUCCESS;
}


const SerialisableMap<SsvId, Value<Point> >
Agent::RangeQueryPoint(const Point start, const Point end, unsigned long timestamp) {
  assert(timestamp > this->GetLVT());
  const RangeQueryMessage *ret = SendRangeQueryPointMessageAndGetResponse(timestamp, start, end);
  auto r = ret->GetSsvValueList();
  this->SetLVT(timestamp);

  return r;
}

void Agent::NotifyMessageArrive() {
  this->message_waiting_sem_.Signal();
}

void Agent::ResetMessageArriveSemaphore() {
  this->message_waiting_sem_.Reset();
}




