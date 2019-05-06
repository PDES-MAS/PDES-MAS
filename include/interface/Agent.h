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
#include <csignal>

namespace pdesmas {
  class Agent : public Thread {

  private:
    void WaitUntilMessageArrive();

    const AbstractMessage *Read(int, unsigned long);

    const AbstractMessage *WriteInt(int, int, unsigned long);

    const AbstractMessage *WriteDouble(int, double, unsigned long);

    const AbstractMessage *WritePoint(int, const Point, unsigned long);

    const AbstractMessage *WriteString(int, const string, unsigned long);

    const AbstractMessage *RangeQuery(unsigned long, const Point, const Point);


    bool SetLVT(unsigned long lvt);

    Alp *attached_alp_;
    LpId agent_identifier_;
    unsigned long start_time_;
    unsigned long end_time_;

    void *MyThread(void *) final;

  protected:
    void SendGVTMessage();

    void SendEndMessage();

    unsigned long GetLVT();


  public:
    Agent(unsigned long const start_time, unsigned long const end_time, Alp *parent_alp, unsigned long agent_id);

    virtual void Cycle() = 0;

    void Finalise();
  };
}

#endif /* IAGENT_H_ */
