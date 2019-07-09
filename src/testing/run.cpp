#include "Simulation.h"
#include "TestAgent.h"
#include <iostream>
#include "spdlog/spdlog.h"

using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {

  Simulation sim = Simulation();
  sim.Construct(7, 8, 0, 100000);
  spdlog::info("Process up, rank {0}, size {1}", sim.rank(), sim.size());
  sim.attach_alp_to_clp(7, 3)
      .attach_alp_to_clp(8, 3)
      .attach_alp_to_clp(9, 4)
      .attach_alp_to_clp(10, 4)
      .attach_alp_to_clp(11, 5)
      .attach_alp_to_clp(12, 5)
      .attach_alp_to_clp(13, 6)
      .attach_alp_to_clp(14, 6)
      .Initialise();
  
  TestAgent test = TestAgent(0, 0, nullptr, sim.rank());
  sim.Finalise();
}