#include "Simulation.h"
#include "TestAgent.h"
#include <iostream>
using namespace std;
using namespace pdesmas;

int main(int argc, char **argv) {
  Simulation sim = Simulation();
  cout<<"Process up, rank "<<sim.rank()<<endl;
  sim.Construct(7,8,0,100000);

  TestAgent test = TestAgent(0, 0, nullptr, sim.rank());

}