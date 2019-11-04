//
// Created by pill on 19-6-26.
//

#include "TileWorldAgent.h"
#include <spdlog/spdlog.h>
#include <unistd.h>
#include <random>

TileWorldAgent::TileWorldAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId,
                               unsigned int world_size_w, unsigned int world_size_h, unsigned int sense_range) : Agent(
    startTime, endTime, agentId), kSenseRange(sense_range) {
  this->WritePrivateInt(IS_TILE_CARRYING, 0);
  this->WritePrivateInt(IS_EN_ROUTE, 0);

}


static inline int GetFirstDigit(int number) {
  while (number >= 10) {
    number = (number - (number % 10)) / 10;
  }
  return number;
}

void TileWorldAgent::Cycle() {
  //spdlog::warn("Cycle begin");

  // where am i?
  //spdlog::debug("Agent {0}, read {0}",this->agent_id());
  //Point my_position=Point(0,0);
  Point my_position = this->ReadPoint(this->agent_id(), this->GetLVT() + 1);
  int curr_x = my_position.GetX();
  int curr_y = my_position.GetY();
  spdlog::debug("Agent {0}, at ({1},{2})", this->agent_id(), curr_x, curr_y);
  //sense
  SerialisableMap<SsvId, Value<Point> > results = this->RangeQueryPoint(
      Point(curr_x - kSenseRange, curr_y - kSenseRange),
      Point(curr_x + kSenseRange, curr_y + kSenseRange),
      this->GetLVT() + 1);
//  SerialisableMap<SsvId, Value<Point> > results = this->RangeQueryPoint(
//      Point(-100,-100),
//      Point(100,100),
//      this->GetLVT());
  spdlog::debug("RQ result size: {0}", results.size());
  //spdlog::debug("Agent {0}, Agent LVT {1}, preparing to read id {2}", this->agent_id(), this->GetLVT(), 1);
  map<SsvId, Point> hole_in_range = map<SsvId, Point>();
  map<SsvId, Point> tile_in_range = map<SsvId, Point>();
  map<SsvId, Point> obstacle_in_range = map<SsvId, Point>();
  for (auto &i :results) {
    SsvId ssv_id = i.first;
    Point p = i.second.GetValue();
    switch (GetObjectTypeFromSsvId(ssv_id)) {
      case AGENT:
        // no use for now
        break;
      case HOLE:
        // fill if carrying tiles
        // record for further use
        hole_in_range.insert(make_pair(ssv_id, p));
        break;
      case TILE:
        // pick if no tiles carrying
        // record for further use
        tile_in_range.insert(make_pair(ssv_id, p));

        break;
      case OBSTACLE:
        // temporarily of no use, just to check reachability
        obstacle_in_range.insert(make_pair(ssv_id, p));

        break;
      default:
        break;
    }
  }
  // TODO: find closest object maybe?
  for (auto &i:obstacle_in_range) {
    SsvId ssv_id = i.first;
    Point p = i.second;
    //we should ues A* here, but seems it makes no difference, just pass
  }
  for (auto &i:tile_in_range) {
    SsvId ssv_id = i.first;
    Point p = i.second;
    if (ReadPrivateInt(IS_TILE_CARRYING) == 0) {
      //pick up tile
      WritePrivateInt(IS_TILE_CARRYING, 1);
      //WritePoint(this->agent_id(), p, this->GetLVT() + 10); // moving to the position
      // FIXME: count the distance with A*
      break;
    }

  }
  for (auto &i:hole_in_range) {
    SsvId ssv_id = i.first;
    Point p = i.second;
    if (ReadPrivateInt(IS_TILE_CARRYING) == 1) {
      // have tile, move to hole
      WritePrivateInt(IS_TILE_CARRYING, 0);
      //WritePoint(this->agent_id(), p, this->GetLVT() + 10);

      break;
    }
  }

  if (agent_id() == 10701) {
    // GVT initiator
    if (this->GetLVT() > this->GetGVT() + 2000) {
      this->SendGVTMessage();
      spdlog::debug("GVT {}", this->GetGVT());


    }
  }
  //this->Sleep(100);
  this->time_wrap(1000);

  //spdlog::warn("Cycle end");

}

inline TileWorldAgent::Object TileWorldAgent::GetObjectTypeFromSsvId(SsvId &ssv_id) {
  if (GetFirstDigit(ssv_id.id()) == 1) {
    return AGENT;
  } else if (GetFirstDigit(ssv_id.id()) == 2) {
    return HOLE;
  } else if (GetFirstDigit(ssv_id.id()) == 3) {
    return TILE;
  } else if (GetFirstDigit(ssv_id.id()) == 4) {
    return OBSTACLE;
  }
  return NUL;

}
