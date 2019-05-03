//
// Created by pill on 19-4-23.
//

#ifndef PDES_MAS_SIMULATION_H
#define PDES_MAS_SIMULATION_H


#include <lp/Clp.h>
#include <Alp.h>
#include <interface/IAgent.h>

using namespace pdesmas;

class Simulation {
private:
  pdesmas::IAgent *i_agent_;
  pdesmas::Clp *clp_;
  int comm_rank_;
  int comm_size_;
  int number_of_clp_;
  int number_of_alp_;
  unsigned long start_time_;
  unsigned long end_time_;

public:
  Simulation();


  ~Simulation();

  void Construct(int number_of_clp, int number_of_alp, unsigned long start_time, unsigned long end_time);

  void Initialise(const string &config_file_path);

  void Finalise();


  int rank();

  int size();

  unsigned long GVT();
};


#endif //PDES_MAS_SIMULATION_H
