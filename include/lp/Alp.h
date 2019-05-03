/*
 * Alp.h
 *
 *  Created on: 9 Sep 2010
 *      Author: Dr B.G.W. Craenen (b.craenen@cs.bham.ac.uk)
 */

#ifndef IALP_H_
#define IALP_H_

#include "Lp.h"
#include "HasSendList.h"
#include "HasRollbackTagList.h"
#include "SingleReadResponseMessage.h"
#include "WriteResponseMessage.h"
#include <vector>

namespace pdesmas {
  class IAgent;

  class Alp : public Lp, public HasSendList, public HasRollbackTagList {
  private:
    map<int, IAgent *> managed_agents_;
    map<int, unsigned long> agent_lvt_map_;
    unsigned int fParentClp;
    Semaphore fReceiveProcessSemaphore;
    Semaphore fOutsideMessageSemaphore;
    Semaphore fPreResponseSemaphore;
    Mutex fProcessMessageMutex;
    list<unsigned long> fIgnoreIDList;

    bool AddAgent(unsigned long agent_id, IAgent *agent);

    bool ProcessRollback(const RollbackMessage *);

    void ProcessMessage(const RollbackMessage *);

    void ProcessMessage(const SingleReadResponseMessage *);

    void ProcessMessage(const WriteResponseMessage *);

    void ProcessMessage(const RangeQueryMessage *);

  public:
    Alp(unsigned int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long, const Initialisor *);

    unsigned int GetParentClp() const;

    unsigned long GetLvt() const;

    void SetGvt(unsigned long);

    void SignalReceiveProcess();

    void SignalOutsideMessage();

    void SignalPreResponse();

    void SetIgnoreID(unsigned long);

    bool CheckIgnoreID(unsigned long);

    void Send() override ;

    void Receive() override ;

    void Initialise() override;

    void Finalise() override ;
  };
}

#endif /* IALP_H_ */
