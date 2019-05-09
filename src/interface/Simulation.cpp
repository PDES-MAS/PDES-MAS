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
  int depth = (int) (log(comm_size_) / log(2));
  int clp_max_rank = int(pow(2, depth - 1)) - 2;
  int alp_max_rank = int(pow(2, depth)) - 2;
  auto *initialisor = new Initialisor();

  if (comm_rank_ <= clp_max_rank) { // this instance is CLP
    initialisor->ParseFileCLP(config_file_path, comm_rank_);
    auto clp = new Clp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor);
    clp->Run();
  } else if (comm_rank_ <= alp_max_rank) { // is alp
    initialisor->ParseFileALP(config_file_path);
    alp_ = new Alp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor);
  } else {
    printf("Unused process, rank=%d\n", comm_rank_);
    exit(0);
  }
}

void Simulation::Finalise() {
  if (comm_rank_ >= number_of_clp_) { // is alp
    //i_agent_->SendEndMessage();
  }
  MPI_Finalize();
}

int Simulation::rank() {
  return comm_rank_;
}

int Simulation::size() {
  return comm_size_;
}

unsigned long Simulation::GVT() {
  return 0; //TODO
}