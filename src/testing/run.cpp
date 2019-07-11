#include "Simulation.h"
#include "TestAgent.h"
#include <iostream>
#include "spdlog/spdlog.h"

using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::debug);
  Simulation sim = Simulation();
  sim.Construct(1, 2, 0, 10000);
  spdlog::info("MPI process up, rank {0}, size {1}", sim.rank(), sim.size());
  sim
      .attach_alp_to_clp(1, 0)
      .attach_alp_to_clp(2, 0)
//.attach_alp_to_clp(7, 3)
//      .attach_alp_to_clp(8, 3)
//      .attach_alp_to_clp(9, 4)
//      .attach_alp_to_clp(10, 4)
//      .attach_alp_to_clp(11, 5)
//      .attach_alp_to_clp(12, 5)
//      .attach_alp_to_clp(13, 6)
//      .attach_alp_to_clp(14, 6)
      .Initialise();
  string type = "CLP";
  if (sim.alp() != nullptr) {
    type = "ALP";
  }
  spdlog::info("Initialized, rank {0}, is {1}", sim.rank(), type);

  if (sim.alp() != nullptr) {
    TestAgent *test = new TestAgent(0, 10000, sim.alp(), sim.rank() * 100 + 1);
    sim.alp()->AddAgent(test->get_id().GetId(), test);

  }
  spdlog::info("Agent added, rank {0}", sim.rank());

  sim.Run();
  spdlog::info("LP exit, rank {0}", sim.rank());

  sim.Finalise();
}