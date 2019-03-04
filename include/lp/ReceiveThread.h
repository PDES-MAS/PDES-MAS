#ifndef _RECEIVETHREAD_H_
#define _RECEIVETHREAD_H_

#include <mpi.h>
#include "LpId.h"
#include "AbstractMessage.h"
#include "Thread.h"

namespace pdesmas {
  class ReceiveThread: public Thread {
    private:
      Lp* fLp;
      MpiInterface* fMPIInterface;
      bool fIsSimulationRunning;
    public:
      ReceiveThread(Lp*, MpiInterface*);
      ~ReceiveThread();
      AbstractMessage* Receive(MPI_Status*) const;
      void* MyThread(void*);
      void StopSimulation();
  };

}
#endif
