//
// Created by pill on 19-6-26.
//

#include "TestAgent.h"
#include <spdlog/spdlog.h>
#include <unistd.h>
TestAgent::TestAgent(const unsigned long startTime, const unsigned long endTime, Alp *parentAlp,
                     unsigned long agentId) : Agent(startTime, endTime, parentAlp, agentId) {

}

void TestAgent::Cycle() {
  this->time_wrap(1000);
  spdlog::debug("Agent {0}, LVT {1}", this->get_id().GetId(), this->GetLVT());
  this->SendGVTMessage();
  spdlog::debug("Agent {0}, GVT {1}", this->get_id().GetId(), this->GetGVT());

  usleep(1000000);
}