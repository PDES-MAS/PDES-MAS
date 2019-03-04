/*!

 \brief provides the interface of the kernel with MPI

 MpiInterface.h

 Author: Michael Lees (mhl)
 Date: 01/04/2005

 This class provides the interface between LPs and the MPI
 communication infrastructure. At present a single MpiInterface (one
 thread) is used for both sending and receiving messages. Eventually
 with MPI_THREAD_MULTIPLE support this will change.

 Revisions: 30/08/05 - mhl - After some work with assk it was
 discovered the old single thread design is actually unsafe. To remedy
 this a severe re-design was required. Now the send and receive threads
 are separated into two threads, with all MPI calls being locked by a
 mutex. This involves two new classes SendThread and ReceiveThread

 */
#ifndef _MPIINTERFACE_H_
#define _MPIINTERFACE_H_

#include <mpi.h>
#include "LpId.h"
#include "AbstractMessage.h"
#include "Semaphore.h"
#include "Mutex.h"

namespace pdesmas {
  class SendThread;
  class ReceiveThread;
  class MpiInterface {
    private:
      Mutex fMutex;

      SendThread* sendThread; /**< The thread used for sending messages to MPI */
      ReceiveThread* receiveThread; /**< Thread used for receiving messages from MPI */

      Semaphore sendSemaphore;
      Semaphore recvSemaphore;

    public:
      MpiInterface();
      MpiInterface(Lp*, void*);
      virtual ~MpiInterface();
      void StopSimulation();
      void Signal();
      void Join();
      void Stop();

      void Send(AbstractMessage* sendMsg);

      void ReceiveWait();
      void SendWait();
      void ReceiveSignal();
      void SendSignal();

      int LockMpi();
      int UnlockMpi();
  };
}
#endif
