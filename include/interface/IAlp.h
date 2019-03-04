/*
 * IAlp.h
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

namespace pdesmas {
  class IAgent;
  class IAlp: public Lp, public HasSendList, public HasRollbackTagList {
    private:
      IAgent* fIAgent;
      unsigned int fParentClp;
      Semaphore fReceiveProcessSemaphore;
      Semaphore fOutsideMessageSemaphore;
      Semaphore fPreResponseSemaphore;
      Mutex fProcessMessageMutex;
      list<unsigned long> fIgnoreIDList;

      bool ProcessRollback(const RollbackMessage*);

      void ProcessMessage(const RollbackMessage*);
      void ProcessMessage(const SingleReadResponseMessage*);
      void ProcessMessage(const WriteResponseMessage*);
      void ProcessMessage(const RangeQueryMessage*);

    public:
      IAlp(unsigned int, unsigned int, unsigned int, unsigned int, unsigned long, unsigned long, const string, IAgent*);

      unsigned int GetParentClp() const;
      unsigned long GetLvt() const;
      void SetGvt(unsigned long);

      void SignalReceiveProcess();
      void SignalOutsideMessage();
      void SignalPreResponse();

      void SetIgnoreID(unsigned long);
      bool CheckIgnoreID(unsigned long);

      void Send();
      void Receive();

      void Initialise();
      void Finalise();
  };
}

#endif /* IALP_H_ */
