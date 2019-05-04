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
#include <interface/IdentifierHandler.h>

namespace pdesmas {
  class Agent;

  class Alp : public Lp, public HasSendList, public HasRollbackTagList {
  private:
    map<unsigned long, Agent *> managed_agents_;
    map<unsigned long, unsigned long> agent_lvt_map_;
    IdentifierHandler *message_id_handler_;
    map<unsigned long, const AbstractMessage *> agent_response_;


    int fParentClp;
    Mutex fProcessMessageMutex;


    bool ProcessRollback(const RollbackMessage *);

    void ProcessMessage(const RollbackMessage *);

    void ProcessMessage(const SingleReadResponseMessage *);

    void ProcessMessage(const WriteResponseMessage *);

    void ProcessMessage(const RangeQueryMessage *);

  public:
    Alp(unsigned int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long, const Initialisor *);

    bool AddAgent(unsigned long agent_id, Agent *agent);

    unsigned int GetParentClp() const;

    unsigned long GetAgentLvt(unsigned long agent_id) const;

    bool HasAgent(unsigned long agent_id);

    unsigned long GetLvt() const;

    unsigned long GetNewMessageId() const;

    const AbstractMessage *GetResponseMessage(unsigned long agent_id) const;

    void WaitForResponseMessageToArrive(Semaphore &sem);

    void SetGvt(unsigned long);

    void Send() override;

    void Receive() override;

    void Initialise() override;

    void Finalise() override;
  };
}

#endif /* IALP_H_ */
