//
// Created by pill on 19-6-26.
//

#ifndef PDES_MAS_TILEWORLDAGENT_H
#define PDES_MAS_TILEWORLDAGENT_H

#include <interface/Agent.h>

using namespace pdesmas;
using namespace std;

class TileWorldAgent : public Agent {
public:
  enum Object {
    HOLE, AGENT, TILE, OBSTACLE, NUL
  };
  enum {
    IS_TILE_CARRYING = 1,
    IS_EN_ROUTE = 2,

  };

  TileWorldAgent(unsigned long start_time, unsigned long end_time, unsigned long agent_id,
                 unsigned int world_size_w, unsigned int world_size_h, unsigned int sense_range);

  void Cycle() override;

  Object GetObjectTypeFromSsvId(SsvId &ssv_id);

private:

  unsigned int kSenseRange;
  unsigned long kLocationSsvId;
};


#endif //PDES_MAS_TILEWORLDAGENT_H
