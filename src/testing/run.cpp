#include "Simulation.h"
#include "TestAgent.h"
#include <iostream>
#include "spdlog/spdlog.h"

using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::debug);
  Simulation sim = Simulation();
//  sim.Construct(1, 2, 0, 10000);
  sim.Construct(7, 8, 0, 10000);

  spdlog::info("MPI process up, rank {0}, size {1}", sim.rank(), sim.size());
  sim
//      .attach_alp_to_clp(1, 0)
//      .attach_alp_to_clp(2, 0)
      .attach_alp_to_clp(7, 3)
      .attach_alp_to_clp(8, 3)
      .attach_alp_to_clp(9, 4)
      .attach_alp_to_clp(10, 4)
      .attach_alp_to_clp(11, 5)
      .attach_alp_to_clp(12, 5)
      .attach_alp_to_clp(13, 6)
      .attach_alp_to_clp(14, 6)
      .preload_variable(0,1)
      .preload_variable(1,1.2f)
      .preload_variable(2,"1234")
      .preload_variable(3,Point(0,3))
      .preload_variable(4,Point(0,4))
      .preload_variable(5,Point(0,5))
      .preload_variable(6,Point(0,6))
      .Initialise();

  spdlog::info("Initialized, rank {0}, is {1}", sim.rank(), sim.type());
  if (sim.type()=="ALP") {
    for (int i = 0; i < 4; ++i) {
      TestAgent *test = new TestAgent(0, 10000, sim.rank() * 100 + 1 + i);
      sim.add_agent(test);
    }

  }

  sim.Run();
  spdlog::info("LP exit, rank {0}", sim.rank());

  sim.Finalise();
}