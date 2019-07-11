//
// Created by pill on 19-6-26.
//

#include "TestAgent.h"
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <random>

TestAgent::TestAgent(const unsigned long startTime, const unsigned long endTime, Alp *parentAlp,
                     unsigned long agentId) : Agent(startTime, endTime, parentAlp, agentId) {

}

void TestAgent::Cycle() {
  if (this->GetEndTime() - this->GetLVT() <= 1000) {
    this->time_wrap(this->GetEndTime() - this->GetLVT());
  } else {
    this->time_wrap(random() % (500) + 500);
  }
  spdlog::debug("Agent {0}, Agent LVT {1}", this->get_id().GetId(), this->GetLVT());
  spdlog::debug("Agent {0}, ALP LVT {1}", this->get_id().GetId(), this->GetAlpLVT());
  spdlog::debug("Agent {0}, Send GVT message", this->get_id().GetId());
  this->SendGVTMessage();
  spdlog::debug("Agent {0}, GVT {1}", this->get_id().GetId(), this->GetGVT());

  sleep(1);
}