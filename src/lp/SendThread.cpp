#include <sstream>
#include <mpi.h>
#include "MpiInterface.h"
#include "SendThread.h"
#include "Lp.h"
#include "ObjectMgr.h"
#include "WriteMessage.h"
#include "SingleReadResponseMessage.h"
#include <spdlog/spdlog.h>
using namespace std;
using namespace pdesmas;

SendThread::SendThread(Lp* pLp, MpiInterface* pMPIInterface) :
  fIsSimulationRunning(true), fLp(pLp), fMPIInterface(pMPIInterface) {
  Start(this);
}

SendThread::~SendThread() {
  Stop();
}

void SendThread::Send(AbstractMessage* sendMessage) {
  // Declare output string stream
  ostringstream serialisedMessageStream(ostringstream::out);
  //Serialise message into stream
  sendMessage->Serialise(serialisedMessageStream);
  //Convert stream into string
  string serialisedMessage = serialisedMessageStream.str();
  //Get string length
  size_t stringLength = strlen(serialisedMessage.c_str()) + 1;
  // Declare byte array for message
  char* sendBuffer = new char[stringLength];
  // Copy serialised message into byte array
  strcpy(sendBuffer, serialisedMessage.c_str());
  // Declare tag and MPI request variable
  int send_tag = 0;
  MPI_Request mpiRequest;
  // Send message through MPI
  MPI_Isend(sendBuffer, (int) stringLength, MPI_BYTE, sendMessage->GetDestination(), send_tag, MPI_COMM_WORLD, &mpiRequest);
  // Declare send flag and MPI status
  int sendFlag = 0;
  MPI_Status mpiStatus;
  // Wait until the send message part has been completed
  while (sendFlag == 0 && fIsSimulationRunning) {
    MPI_Test(&mpiRequest, &sendFlag, &mpiStatus);
  }
  // Message has been send free memory for the send buffer
  delete[] sendBuffer;
  // Message has been send. Will need to free the memory for value first.
  switch (sendMessage->GetType()) {
    case SINGLEREADRESPONSEMESSAGE:
      static_cast<SingleReadResponseMessage*> (sendMessage)->ClearValue();
      break;
    case WRITEMESSAGE:
      static_cast<WriteMessage*> (sendMessage)->ClearValue();
      break;
    default:
      // skip
      break;
  }
  // Free memory for the send message itself
  delete sendMessage;
}

void* SendThread::MyThread(void* arg) {
  //Wait to be signalled at startup
  this->Wait();
  while (fIsSimulationRunning) {
    //Wait on semaphore until we're signalled to send
    fMPIInterface->SendWait();

    if (fIsSimulationRunning) {
      fMPIInterface->LockMpi();
      fLp->Send();
      fMPIInterface->UnlockMpi();
    }
  }
  pthread_exit(0);
}

void SendThread::StopSimulation() {
  fIsSimulationRunning = false;
}
