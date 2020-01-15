#include "Simulation.h"
#include "NaiveAgent.h"
#include <iostream>
#include "spdlog/spdlog.h"

using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::debug);
  Simulation sim = Simulation();
  uint64_t numAgents = std::atoll(argv[1]);
  uint64_t numMPI = std::atoll(argv[2]);


  // numMPI -> CLP and ALP
  uint64_t numALP = (numMPI + 1) / 2;
  uint64_t numCLP = numALP - 1;
  spdlog::debug("CLP: {}, ALP: {}", numCLP, numALP);
  sim.Construct(numCLP, numALP, 0, 10000);
  spdlog::info("MPI process up, rank {0}, size {1}", sim.rank(), sim.size());
  list<unsigned long> agIdList;

  // attach alp to clp
  for (uint64_t i = numCLP; i < numMPI; ++i) {
    sim.attach_alp_to_clp(i, (i - 1) / 2);
    //spdlog::info("attached alp{0} to clp{1}", i, (i - 1) / 2);

  }
  for (uint64_t i = numCLP; i < numMPI; ++i) {
    for (uint64_t j = 0; j < numAgents / numALP; ++j) {
      // preload mailbox variable
      unsigned long agentId = 10000 + i * 100 + 1 + j;
      agIdList.push_back(agentId);
      sim.preload_variable(agentId, "", 0);
    }
  }


  sim.Initialise();
  spdlog::info("Initialized, rank {0}, is {1}", sim.rank(), sim.type());

  if (sim.type() == "ALP") {
    for (uint64_t i = 0; i < numAgents / numALP; ++i) {
      unsigned long agentId = 10000 + sim.rank() * 100 + 1 + i;

      NaiveAgent *nvAg = new NaiveAgent(0, 10000, agentId);
      nvAg->InitSendList(agIdList, 5, 114514);
      // TODO init sendList after generating all agents
      // list<unsigned long> agList, unsigned int listLen, unsigned int seed
      sim.add_agent(nvAg);
      spdlog::debug("Agent added, rank {}, id {}", sim.rank(), agentId);
    }
  }

  sim.Run();

  spdlog::info("LP exit, rank {0}", sim.rank());

  sim.Finalise();

}