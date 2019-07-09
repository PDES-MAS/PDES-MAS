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
  for (int i = 0; i < number_of_alp + number_of_clp; ++i) {
    topology_[i] = new DummyNode();
  }
  initialisor_ = new Initialisor();
}

Simulation &Simulation::set_topology(const string &topo) {
  return *this;
}

Simulation &Simulation::attach_node(int from_rank, Port from_port, int to_rank, Port to_port) {
  auto from_dummynode = topology_[from_rank];
  auto to_dummynode = topology_[to_rank];
  if ((*from_dummynode)[from_port] == nullptr && (*to_dummynode)[to_port] == nullptr) {
    // ports are not connected
    from_dummynode->set(from_port, to_dummynode);
    to_dummynode->set(to_port, from_dummynode);
  }
  return *this;
}

Simulation &Simulation::attach_alp_to_clp(int alp_rank, int clp_rank) {
  initialisor_->attach_alp_to_clp(alp_rank, clp_rank);
  return *this;
}


void Simulation::Initialise() {
  MPI_Barrier(MPI_COMM_WORLD);

  int clp_max_rank = number_of_clp_ - 1;
  int alp_max_rank = clp_max_rank + number_of_alp_;
#ifdef PDESMAS_DEBUG

#endif
  if (comm_rank_ <= clp_max_rank) { // this instance is CLP
    clp_ = new Clp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor_);
    clp_->Run();
  } else if (comm_rank_ <= alp_max_rank) { // is alp
    alp_ = new Alp(comm_rank_, comm_size_, number_of_clp_, number_of_alp_, start_time_, end_time_, initialisor_);
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

Simulation::Simulation() {

}

Simulation::~Simulation() {

}
