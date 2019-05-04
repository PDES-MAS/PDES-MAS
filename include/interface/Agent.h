/*
 * Agent.h
 *
 *  Created on: 4 May 2019
 *      Author: CubicPill
 */

#ifndef IAGENT_H_
#define IAGENT_H_

#include "Alp.h"
#include "IdentifierHandler.h"
#include "Thread.h"
#include "HasIDLVTMap.h"
#include "HasMPIInfo.h"
#include "HasResponseMessage.h"
#include "EndMessage.h"

namespace pdesmas {
  class Agent {
  private:
    Alp *attached_alp_;
    LpId identifier_;
    Semaphore semaphore_has_response_;
    bool rollback_happened_ = false;

  protected:

    unsigned long GetGvt() const;

    const AbstractMessage *Read(long, int, unsigned long);

    const AbstractMessage *WriteInt(long, int, int, unsigned long);

    const AbstractMessage *WriteDouble(long, int, double, unsigned long);

    const AbstractMessage *WritePoint(long, int, const Point, unsigned long);

    const AbstractMessage *WriteString(long, int, const string, unsigned long);

    const AbstractMessage *RangeQuery(long, unsigned long, const Point, const Point);

    void SendGVTMessage();

    void SendEndMessage();

    void WaitForMessage();

  public:
    Agent(unsigned long const &start_time, unsigned long const &end_time, Alp *parent_alp, unsigned long agent_id);

    unsigned long GetTime();

    void Finalise();
  };
}

#endif /* IAGENT_H_ */
