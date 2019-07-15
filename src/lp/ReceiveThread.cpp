#include "MpiInterface.h"
#include "ReceiveThread.h"
#include <sstream>
#include <mpi.h>
#include "Lp.h"
#include "ObjectMgr.h"
#include <spdlog/spdlog.h>

using namespace std;

ReceiveThread::ReceiveThread(Lp *pLp, MpiInterface *pMPIInterface)
    : fLp(pLp), fMPIInterface(pMPIInterface), fIsSimulationRunning(true) {
  Start(this);
}

ReceiveThread::~ReceiveThread() {
  Stop();
}

AbstractMessage *ReceiveThread::Receive(MPI_Status *pStatus) const {
  // Declare receiveLength integer
  int receiveLength;
  //Get the size of the received message
  MPI_Get_count(pStatus, MPI_BYTE, &receiveLength);

  // FIXME: MPI Probe will cause problem here (message truncated), that's only a temp fix
  receiveLength = (receiveLength > 2048) ? receiveLength : 2048;
  //Allocate the receive buffer
  char *receiveBuffer = new char[receiveLength];
  //Call MPI_Recv
  MPI_Recv(receiveBuffer, receiveLength, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, pStatus);
  //Convert to string
  string serialisedMessage = receiveBuffer;
  //Convert to stream
  istringstream serialisedMessageStream(serialisedMessage, istringstream::in);
  //Use the AbstractPool to recreate an instance on the heap
  AbstractMessage *receivedMessage = messageClassMap->CreateObject(GetTypeID(serialisedMessage));
  //Deserialise the string to fill in all message fields.
  receivedMessage->Deserialise(serialisedMessageStream);
  // Release memory for the receive buffer
  delete[] receiveBuffer;
  // Return received message
  return receivedMessage;
}

void *ReceiveThread::MyThread(void *arg) {
  //Wait to be signalled at startup
  this->Wait();
  // Run this thread while the simulation is running
  while (fIsSimulationRunning) {
    // Lock the MPI mutex
    fMPIInterface->LockMpi();
    // Declare the probe flag
    int probeFlag = 0;
    // Declare the mpiStatus variable
    MPI_Status mpiStatus = MPI_Status();
    // Probe MPI to see if a message is ready
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &probeFlag, &mpiStatus);
    // Unlock the MPI mutex
    fMPIInterface->UnlockMpi();
    // Check the probe flag
    if (probeFlag) {
      // If there's a message lock the MPI mutex
      fMPIInterface->LockMpi();
      // Lock the LP
      fLp->Lock();
      // Receive the message through the MPI interface
      AbstractMessage *receivedMessage = Receive(&mpiStatus);
      // Put the message on the receive message queue
      receivedMessage->ReceiveToLp(fLp);
      // Unlock the LP
      fLp->Unlock();
      // Unlock the MPI mutex
      fMPIInterface->UnlockMpi();
      // Signal a receive in the MPI interface
      fMPIInterface->ReceiveSignal();
      // Yield the thread after the message has been received
      sched_yield();
    } else {
      // Yield the thread if there's no message
      sched_yield();
    }
  }
  // Simulation is no longer running, exit this thread
  pthread_exit(0);
}

void ReceiveThread::StopSimulation() {
  // Set is simulation running boolean to false
  fIsSimulationRunning = false;
}
