//
// Created by pill on 19-6-26.
//

#include "TestAgent.h"
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <random>

TestAgent::TestAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId) : Agent(
    startTime, endTime, agentId) {

}

void TestAgent::Cycle() {
  //spdlog::debug("Agent {0}, Agent LVT {1}, preparing to read id {2}", this->agent_id(), this->GetLVT(), 1);
//  SerialisableMap<SsvId, Value<Point> > results = this->RangeQueryPoint(
//      Point(-100, -100),
//      Point(100, 100),
//      this->GetLVT()+1);
  //spdlog::debug("RQ result size: {0}", results.size());
  this->time_wrap((random() % 200));

  this->ReadPoint(this->agent_id(), this->GetLVT() + 1);
  this->ReadPoint(111, this->GetLVT() + 1);
  this->WritePoint(111, Point(random() % 10, random() % 10), this->GetLVT() + 1);
  double v = this->ReadDouble(1, this->GetLVT() + 1);
  spdlog::debug("Agent {0}, Agent LVT {1}, read {2}", this->agent_id(), this->GetLVT(), v);
  if (this->agent_id() == 1103) {
    double vv = (random() % 500) / 1000.0;
    bool ret = this->WriteDouble(1, vv, this->GetLVT() + 1);
    if (ret) {
      spdlog::debug("Agent {0}, Agent LVT {1}, write success: {2} = {3}", this->agent_id(), this->GetLVT(), 1, vv);

    } else {
      spdlog::warn("Agent {0}, Agent LVT {1}, write failed: {2} = {3}", this->agent_id(), this->GetLVT(), 1, vv);

    }
  }

  if (this->GetEndTime() - this->GetLVT() <= 1000) {
    this->time_wrap(this->GetEndTime() - this->GetLVT());
  } else {
    this->time_wrap((random() % 500) + 500);
  }
  spdlog::debug("Agent {0}, Agent LVT {1}", this->agent_id(), this->GetLVT());
  //spdlog::debug("Agent {0}, ALP LVT {1}", this->agent_id(), this->GetAlpLVT());
  //spdlog::debug("Agent {0}, Send GVT message", this->agent_id());
  this->SendGVTMessage();
  spdlog::debug("Agent {0}, GVT {1}", this->agent_id(), this->GetGVT());
}