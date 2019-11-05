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
#include <PrivateVariableStorage.h>

namespace pdesmas {
  class Agent;

  class Alp : public Lp, public HasSendList, public HasRollbackTagList {
  private:
    map<unsigned long, Agent *> managed_agents_;
    map<unsigned long, unsigned long> agent_lvt_map_;
    IdentifierHandler *message_id_handler_;
    map<unsigned long, const AbstractMessage *> agent_response_map_;
    map<unsigned long, unsigned long> agent_response_message_id_map_;
    //map<unsigned long, Semaphore *> agent_waiting_semaphore_map_;
    map<unsigned long, vector<unsigned long> > agent_lvt_history_map_; // use this to perform LVT rollback
    map<unsigned long, PrivateVariableStorage> agent_local_variables_map_;
    map<unsigned long, bool> agent_cancel_flag_map_;
    int fParentClp;
    Mutex fProcessMessageMutex;


    bool ProcessRollback(const RollbackMessage *);

    void ProcessMessage(const RollbackMessage *);

    void ProcessMessage(const SingleReadResponseMessage *);

    void ProcessMessage(const WriteResponseMessage *);

    void ProcessMessage(const RangeQueryMessage *);

  public:
    Alp(unsigned int pRank, unsigned int pCommSize,
        unsigned int pNumberOfClps, unsigned int pNumberOfAlps,
        unsigned long pStartTime, unsigned long pEndTime,
        const Initialisor *initialisor);

    bool AddAgent(Agent *agent);

    void SetCancelFlag(unsigned long agent_id, bool flag);

    bool GetCancelFlag(unsigned long agent_id);

    int GetParentClp() const;

    unsigned long GetAgentLvt(unsigned long agent_id) const;

    bool SetAgentLvt(unsigned long agent_id, unsigned long lvt);

    bool HasAgent(unsigned long agent_id);

    void StartAllAgents();

    void SendEndMessage();

    unsigned long GetLvt() const;

    unsigned long GetNewMessageId() const;

    const AbstractMessage *GetResponseMessage(unsigned long agent_id) const;

    Semaphore &GetWaitingSemaphore(unsigned long agent_id);

    bool TerminationCondition() const override;

    void SetGvt(unsigned long);

    void Send() override;

    void Receive() override;

    void Initialise() override;

    void Finalise() override;
  };
}

#endif /* IALP_H_ */
