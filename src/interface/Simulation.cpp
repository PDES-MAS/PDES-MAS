//
// Created by pill on 19-4-23.
//

#include <parse/Initialisor.h>
#include "Simulation.h"

void Simulation::Construct(int number_of_clp, int number_of_alp, unsigned long start_time, unsigned long end_time) {
  number_of_clp_ = number_of_clp;
  number_of_alp_ = number_of_alp;
  start_time_ = start_time;
  end_time_ = end_time;
  int providedThreadSupport;
  MPI_Init_thread(nullptr, nullptr, MPI_THREAD_SERIALIZED, &providedThreadSupport);
  assert(providedThreadSupport == MPI_THREAD_SERIALIZED);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size_);
  MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank_);
}

void Simulation::Initialise(const string &config_file_path) {
  MPI_Barrier(MPI_COMM_WORLD);

  auto *initialisor = new Initialisor();

  if (comm_rank_ < number_of_clp_) {
    initialisor->ParseFileCLP(config_file_path, comm_rank_);
    clp_ = new Clp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor);
    clp_->Run();
  } else {
    initialisor->ParseFileALP(config_file_path);
    i_agent_ = new IAgent(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor);
  }
}

void Simulation::Finalise() {
  if (comm_rank_ >= number_of_clp_) { // is alp
    i_agent_->SendEndMessage();
  }
  MPI_Finalize();
}