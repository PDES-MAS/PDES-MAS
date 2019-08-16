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
  double v=this->ReadDouble(1,this->GetLVT());
  spdlog::debug("Agent {0}, Agent LVT {1}, read {2}", this->get_id(), this->GetLVT(),v);
  if (this->GetEndTime() - this->GetLVT() <= 1000) {
    this->time_wrap(this->GetEndTime() - this->GetLVT());
  } else {
    this->time_wrap(random() % (500) + 500);
  }
  spdlog::debug("Agent {0}, Agent LVT {1}", this->get_id(), this->GetLVT());
  spdlog::debug("Agent {0}, ALP LVT {1}", this->get_id(), this->GetAlpLVT());
  spdlog::debug("Agent {0}, Send GVT message", this->get_id());
  this->SendGVTMessage();
  spdlog::debug("Agent {0}, GVT {1}", this->get_id(), this->GetGVT());

  sleep(1);
}