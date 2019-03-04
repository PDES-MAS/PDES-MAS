#include <sstream>
#include <mpi.h>
#include "MpiInterface.h"
#include "Lp.h"
#include "ObjectMgr.h"
#include "ReceiveThread.h"
#include "SendThread.h"
#include "Log.h"

using namespace pdesmas;
using namespace std;

MpiInterface::MpiInterface() {
  // Default constructor
}

MpiInterface::MpiInterface(Lp* pLp, void* pArguments) {
  fMutex = Mutex();

  sendThread = new SendThread(pLp, this);
  receiveThread = new ReceiveThread(pLp, this);
  
  sendThread->Signal();
  receiveThread->Signal();
}

MpiInterface::~MpiInterface() {
  // Default deconstructor
}

int MpiInterface::LockMpi(){
  return fMutex.Lock();
}

int MpiInterface::UnlockMpi(){
  return fMutex.Unlock();
}


void MpiInterface::Signal() {
  receiveThread->Signal();
  sendThread->Signal();
}

void MpiInterface::Send(AbstractMessage* sendMsg){
  sendThread->Send(sendMsg);
}

void MpiInterface::StopSimulation(){
  receiveThread->StopSimulation();
  sendThread->StopSimulation();
  SendSignal();
}

void MpiInterface::Join(){
  receiveThread->Join();
  sendThread->Join();
}

void MpiInterface::Stop(){
  receiveThread->Stop();
  sendThread->Stop();
}

void MpiInterface::ReceiveWait() {
  recvSemaphore.Wait();
}

void MpiInterface::SendWait() {
  sendSemaphore.Wait();
}

void MpiInterface::ReceiveSignal() {
  recvSemaphore.Signal();
}

void MpiInterface::SendSignal() {
  sendSemaphore.Signal();
}
