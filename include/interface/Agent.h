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
//#include "HasIDLVTMap.h"
#include "HasMPIInfo.h"
#include "HasResponseMessage.h"
#include "EndMessage.h"
#include "PrivateVariableStorage.h"
#include <csignal>
#include "ThreadWrapper.h"

namespace pdesmas {
  class Agent : public ThreadWrapper {

  private:
    void WaitUntilMessageArrive();

    const SingleReadResponseMessage *SendReadMessageAndGetResponse(unsigned long variable_id, unsigned long timestamp);

    template<typename T>
    const WriteResponseMessage *
    SendWriteMessageAndGetResponse(unsigned long pVariableId, T pValue, unsigned long pTime);


    const RangeQueryMessage *
    SendRangeQueryPointMessageAndGetResponse(unsigned long pTime, const Point pStartValue, const Point pEndValue);


    bool SetLVT(unsigned long lvt);

    Alp *attached_alp_ = nullptr;
    LpId agent_identifier_;
    Semaphore message_waiting_sem_;
    unsigned long start_time_;
    unsigned long end_time_;
    unsigned long agent_id_;

    PrivateVariableStorage *private_variable_storage_;

    void Body() final;

  protected:
    void SendGVTMessage();


    void time_wrap(unsigned long t);

    const int ReadInt(unsigned long variable_id, unsigned long timestamp);

    const double ReadDouble(unsigned long variable_id, unsigned long timestamp);

    const Point ReadPoint(unsigned long variable_id, unsigned long timestamp);

    const string ReadString(unsigned long variable_id, unsigned long timestamp);

    const int ReadPrivateInt(unsigned long variable_id);

    const double ReadPrivateDouble(unsigned long variable_id);

    const Point ReadPrivatePoint(unsigned long variable_id);

    const string ReadPrivateString(unsigned long variable_id);

    bool WritePrivateInt(unsigned long variable_id, int v);

    bool WritePrivateDouble(unsigned long variable_id, double v);

    bool WritePrivatePoint(unsigned long variable_id, Point v);

    bool WritePrivateString(unsigned long variable_id, string v);

    bool WriteInt(unsigned long variable_id, int value, unsigned long timestamp);

    bool WriteDouble(unsigned long variable_id, double value, unsigned long timestamp);

    bool WritePoint(unsigned long variable_id, Point value, unsigned long timestamp);

    bool WriteString(unsigned long variable_id, string value, unsigned long timestamp);

    const SerialisableMap<SsvId, Value<Point> >
    RangeQueryPoint(const Point start, const Point end, unsigned long timestamp);


  public:
    Agent(unsigned long const start_time, unsigned long const end_time, unsigned long agent_id);

    void attach_alp(Alp *alp);

    // agent's main loop, must be overridden
    virtual void Cycle() = 0;

    unsigned long agent_id() { return agent_id_; };

    unsigned long GetLVT() const;

    unsigned long GetAlpLVT() const;

    unsigned long GetGVT() const;

    unsigned long GetEndTime() const { return attached_alp_->GetEndTime(); }

    // this could be overridden to do some collection
    virtual void Finalise();

    void NotifyMessageArrive();

    void ResetMessageArriveSemaphore();
  };
}

#endif /* IAGENT_H_ */
