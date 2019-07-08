//
// Created by pill on 19-6-26.
//

#ifndef PDES_MAS_TESTAGENT_H
#define PDES_MAS_TESTAGENT_H

#include <interface/Agent.h>

using namespace pdesmas;
using namespace std;

class TestAgent : public Agent {
  void Cycle() override;
};


#endif //PDES_MAS_TESTAGENT_H
