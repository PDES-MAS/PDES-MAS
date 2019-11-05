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
      .preload_variable(1, 1.1f, 0)
      .preload_variable(111, Point(0, 0), 0)
      .preload_variable(10701, Point(0, 0), 0)
      .preload_variable(10702, Point(1, 0), 0)
      .preload_variable(10801, Point(2, 0), 0)
      .preload_variable(10802, Point(3, 0), 0)
      .preload_variable(10901, Point(4, 0), 0)
      .preload_variable(10902, Point(5, 0), 0)
      .preload_variable(11001, Point(6, 0), 0)
      .preload_variable(11002, Point(7, 0), 0)
      .preload_variable(11101, Point(0, 1), 0)
      .preload_variable(11102, Point(0, 2), 0)
      .preload_variable(11201, Point(0, 3), 0)
      .preload_variable(11202, Point(0, 4), 0)
      .preload_variable(11301, Point(0, 5), 0)
      .preload_variable(11302, Point(0, 6), 0)
      .preload_variable(11401, Point(0, 7), 0)
      .preload_variable(11402, Point(0, 8), 0)
      .Initialise();

  spdlog::info("Initialized, rank {0}, is {1}", sim.rank(), sim.type());
  if (sim.type() == "ALP") {
    for (int i = 0; i < 1; ++i) {
      TestAgent *test = new TestAgent(0, 10000, 10000 + sim.rank() * 100 + 1 + i);
      sim.add_agent(test);
    }

  }

  sim.Run();
  spdlog::info("LP exit, rank {0}", sim.rank());

  sim.Finalise();
}