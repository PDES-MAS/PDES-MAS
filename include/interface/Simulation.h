//
// Created by pill on 19-4-23.
//

#ifndef PDES_MAS_SIMULATION_H
#define PDES_MAS_SIMULATION_H


#include <lp/Clp.h>
#include <Alp.h>
#include <interface/Agent.h>

using namespace pdesmas;


enum Port {
  UP, LEFT, RIGHT
};

class DummyNode {
private:
  DummyNode *U = nullptr;
  DummyNode *R = nullptr;
  DummyNode *L = nullptr;
public:
  DummyNode *operator[](Port p) {
    if (p == Port::UP) {
      return U;
    } else if (p == Port::LEFT) {
      return L;
    } else if (p == Port::RIGHT) {
      return R;
    }
    return nullptr;
  }

  void set(Port p, DummyNode *n) {
    if (p == Port::UP) {
      U = n;
    } else if (p == Port::LEFT) {
      L = n;
    } else if (p == Port::RIGHT) {
      R = n;
    }
  }

};

class Simulation {
private:
  Alp *alp_;
  Clp *clp_;
  int comm_rank_;
  int comm_size_;
  int number_of_clp_;
  int number_of_alp_;
  unsigned long start_time_;
  unsigned long end_time_;
  map<int, DummyNode *> topology_;
  Initialisor *initialisor_;
public:
  Simulation();

  ~Simulation();

  void Construct(int number_of_clp, int number_of_alp, unsigned long start_time, unsigned long end_time);

  void Initialise(const string &config_file_path);

  Simulation &set_topology(const string &topo);

  Simulation &attach_node(int from_rank, Port from_port, int to_rank, Port to_port);

  Simulation &attach_alp_to_clp(int alp_rank, int clp_rank);

  Simulation &preload_variable(SsvId &ssvId, AbstractValue *v);

  void Finalise();

  int rank();

  int size();

  unsigned long GVT();
};


#endif //PDES_MAS_SIMULATION_H
