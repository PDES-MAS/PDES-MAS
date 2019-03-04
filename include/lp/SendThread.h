#ifndef _SENDTHREAD_H_
#define _SENDTHREAD_H_

#include <mpi.h>
#include "LpId.h"
#include "AbstractMessage.h"
#include "Thread.h"

namespace pdesmas {
  class SendThread: public Thread {
    private:
      bool fIsSimulationRunning;
      Lp* fLp;
      MpiInterface* fMPIInterface;

    public:
      SendThread(Lp*, MpiInterface*);
      ~SendThread();

      void Send(AbstractMessage*);
      void* MyThread(void*);
      void StopSimulation();
  };
}
#endif

