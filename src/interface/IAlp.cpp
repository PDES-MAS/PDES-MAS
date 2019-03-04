/*
 * IAlp.cpp
 *
 *  Created on: 9 Sep 2010
 *      Author: Dr B.G.W. Craenen <b.craenen@cs.bham.ac.uk>
 */
#include "assert.h"
#include "IAlp.h"
#include "IAgent.h"
#include "Log.h"
#include "Initialisor.h"
#ifdef TIMER
#include "Helper.h"
#include "FilePrint.h"
#endif
#include "SingleReadAntiMessage.h"
#include "WriteAntiMessage.h"

IAlp::IAlp(unsigned int pRank, unsigned int pCommSize,
    unsigned int pNumberOfClps, unsigned int pNumberOfAlps,
    unsigned long pStartTime, unsigned long pEndTime,
    const string pDataLocation, IAgent* pIAgent) :
  fIAgent(pIAgent) {
  SetRank(pRank);
  SetSize(pCommSize);
  SetNumberOfClps(pNumberOfClps);
  SetNumberOfAlps(pNumberOfAlps);
  fStartTime = pStartTime;
  fEndTime = pEndTime;
  SetGvt(fStartTime);

  fGVTCalculator = new GvtCalculator(this);
  fGVT = fStartTime;

  fReceiveProcessSemaphore = Semaphore();
  fOutsideMessageSemaphore = Semaphore();
  fPreResponseSemaphore = Semaphore();
  fProcessMessageMutex = Mutex(ERRORCHECK);

  Initialisor initialisor = Initialisor();
  initialisor.ParseFileALP(pDataLocation + "Initialisation.dat");
  map<unsigned int, unsigned int>::const_iterator iter =
      initialisor.GetAlpToClpMap().find(GetRank());
  if (iter != initialisor.GetAlpToClpMap().end()) fParentClp = iter->second;
  else {
    LOG(logERROR)
    << "IAlp::IAlp# Couldn't initialise parent rank from initialisation file!";
    exit(1);
  }

  fMPIInterface = new MpiInterface(this, this);

  MPI_Barrier(MPI_COMM_WORLD);

  fMPIInterface->Signal();

  MPI_Barrier(MPI_COMM_WORLD);
}

void IAlp::Send() {
  AbstractMessage* message;
  if (!fSendControlMessageQueue->IsEmpty()) {
    message = fSendControlMessageQueue->DequeueMessage();
  } else if (!fSendMessageQueue->IsEmpty()) {
    message = fSendMessageQueue->DequeueMessage();
    switch (message->GetType()) {
      case SINGLEREADMESSAGE: {
        SingleReadMessage* singleReadMessage = static_cast<SingleReadMessage*>(message);
        AddToSendList(singleReadMessage);
        PreProcessSendMessage(singleReadMessage);
      }
        break;
      case SINGLEREADRESPONSEMESSAGE :
        PreProcessSendMessage(static_cast<SingleReadResponseMessage*> (message));
        break;
      case SINGLEREADANTIMESSAGE :
        PreProcessSendMessage(static_cast<SingleReadAntiMessage*> (message));
        break;
      case WRITEMESSAGE : {
        WriteMessage* writeMessage = static_cast<WriteMessage*>(message);
        AddToSendList(writeMessage);
        PreProcessSendMessage(writeMessage);
      }
        break;
      case WRITERESPONSEMESSAGE :
        PreProcessSendMessage(static_cast<WriteResponseMessage*> (message));
        break;
      case WRITEANTIMESSAGE :
        PreProcessSendMessage(static_cast<WriteAntiMessage*> (message));
        break;
      case RANGEQUERYMESSAGE: {
        RangeQueryMessage* rangeQueryMessage = static_cast<RangeQueryMessage*>(message);
        AddToSendList(rangeQueryMessage);
        PreProcessSendMessage(rangeQueryMessage);
      }
        break;
      case RANGEQUERYANTIMESSAGE :
        PreProcessSendMessage(static_cast<RangeQueryAntiMessage*> (message));
        break;
      case ROLLBACKMESSAGE :
        PreProcessSendMessage(static_cast<RollbackMessage*> (message));
        break;
      default: // skip
        break;
    }
  } else {
    // A message signal was issues, but no message remains to be send, most probably because
    // the message was removed because of a rollback.
    return;
  }
  fMPIInterface->Send(message);
}

void IAlp::Receive() {
  // Fetch received message from the receive queue
  AbstractMessage* message = fReceiveMessageQueue->DequeueMessage();
  fProcessMessageMutex.Lock();
  switch (message->GetType()) {
    case SINGLEREADRESPONSEMESSAGE: {
      SingleReadResponseMessage* singleReadResponseMessage = static_cast<SingleReadResponseMessage*>(message);
      PreProcessReceiveMessage(singleReadResponseMessage);
      ProcessMessage(singleReadResponseMessage);
    }
      break;
    case WRITERESPONSEMESSAGE: {
      WriteResponseMessage* writeResponseMessage = static_cast<WriteResponseMessage*>(message);
      PreProcessReceiveMessage(writeResponseMessage);
      ProcessMessage(writeResponseMessage);
    }
      break;
    case RANGEQUERYMESSAGE: {
      RangeQueryMessage* rangeQueryMessage = static_cast<RangeQueryMessage*>(message);
      PreProcessReceiveMessage(rangeQueryMessage);
      ProcessMessage(rangeQueryMessage);
    }
      break;
    case ROLLBACKMESSAGE: {
      RollbackMessage* rollbackMessage = static_cast<RollbackMessage*>(message);
      PreProcessReceiveMessage(rollbackMessage);
      ProcessMessage(rollbackMessage);
    }
      break;
    case GVTCONTROLMESSAGE:
      fGVTCalculator->ProcessMessage(
          static_cast<const GvtControlMessage*> (message));
      break;
    case GVTREQUESTMESSAGE:
      fGVTCalculator->ProcessMessage(
          static_cast<const GvtRequestMessage*> (message));
      break;
    case GVTVALUEMESSAGE:
      fGVTCalculator->ProcessMessage(
          static_cast<const GvtValueMessage*> (message));
      break;
    default:
      LOG(logERROR)
      << "IAlp::Receive(" << GetRank()
          << ")# Received inappropriate message: " << *message;
      exit(1);
  }
  fProcessMessageMutex.Unlock();
  /*
   * Don't free the memory for value here! That memory _address_ is copied for the interface,
   * and the value is still needed there. Value memory will be freed there.
   */
  delete message;
}

void IAlp::ProcessMessage(const RollbackMessage* pRollbackMessage) {
  fIAgent->Lock();
  if (!ProcessRollback(pRollbackMessage)) {
    fIAgent->Unlock();
    return;
  }
  fIAgent->SetResponseMessage(pRollbackMessage);
  if (!fIAgent->HasResponseMessageWaiting()) {
    LOG(logFINEST)
    << "IAlp::ProcessRollbackMessage(" << GetRank()
        << ")# Outside rollback caught: " << *pRollbackMessage;
    fIAgent->SetOutsideMessageWaiting(true);
    fIAgent->Unlock();
    fOutsideMessageSemaphore.Wait();
    fIAgent->Lock();
    fIAgent->SignalPostOutside();
    fIAgent->Unlock();
    return;
  }
  fIAgent->Unlock();
  fPreResponseSemaphore.Wait();
  fIAgent->Lock();
  fIAgent->SignalResponse();
  fIAgent->Unlock();
  fReceiveProcessSemaphore.Wait();
  fIAgent->Lock();
  fIAgent->SignalPostReceive();
  fIAgent->Unlock();
}

void IAlp::ProcessMessage(
    const SingleReadResponseMessage* pSingleReadResponseMessage) {
  if (CheckIgnoreID(pSingleReadResponseMessage->GetIdentifier())) {
    LOG(logFINEST)
    << "IALP::Receive(" << GetRank()
        << ")# Response message ignored, message: "
        << *pSingleReadResponseMessage;
    return;
  }
  fIAgent->Lock();
  fIAgent->SetResponseMessage(pSingleReadResponseMessage);
  fIAgent->Unlock();
  fPreResponseSemaphore.Wait();
  fIAgent->Lock();
  if (!fIAgent->SignalResponse(pSingleReadResponseMessage->GetIdentifier(),
      pSingleReadResponseMessage->GetOriginalAlp().GetId())) {
    LOG(logWARNING)
    << "IAlp::ProcessSingleReadMessageResponse(" << GetRank()
        << ")# Ignoring message with unsuccessful signal response: "
        << *pSingleReadResponseMessage;
    fIAgent->Unlock();
    return;
  }
  fIAgent->Unlock();
  fReceiveProcessSemaphore.Wait();
  fIAgent->Lock();
  fIAgent->SignalPostReceive();
  fIAgent->Unlock();
}

void IAlp::ProcessMessage(const WriteResponseMessage* pWriteResponseMessage) {
  if (CheckIgnoreID(pWriteResponseMessage->GetIdentifier())) {
    LOG(logFINEST)
    << "IALP::Receive(" << GetRank()
        << ")# Response message ignored, message: " << *pWriteResponseMessage;
    return;
  }
  fIAgent->Lock();
  fIAgent->SetResponseMessage(pWriteResponseMessage);
  fIAgent->Unlock();
  fPreResponseSemaphore.Wait();
  fIAgent->Lock();
  if (!fIAgent->SignalResponse(pWriteResponseMessage->GetIdentifier(),
      pWriteResponseMessage->GetOriginalAlp().GetId())) {
    LOG(logWARNING)
    << "IAlp::ProcessWriteMessageResponse(" << GetRank()
        << ")# Ignoring message with unsuccessful signal response: "
        << *pWriteResponseMessage;
    fIAgent->Unlock();
    return;
  }
  fIAgent->Unlock();
  fReceiveProcessSemaphore.Wait();
  fIAgent->Lock();
  fIAgent->SignalPostReceive();
  fIAgent->Unlock();
}

void IAlp::ProcessMessage(const RangeQueryMessage* pRangeQueryMessage) {
  // Ignore the range query message if we have to
  if (CheckIgnoreID(pRangeQueryMessage->GetIdentifier())) {
    LOG(logFINEST)
    << "IALP::ProcessRangeQueryMessage(" << GetRank()
        << ")# RangeQuery message ignored, message: " << *pRangeQueryMessage;
    return;
  }
  fIAgent->Lock();
  fIAgent->SetResponseMessage(pRangeQueryMessage);
  fIAgent->Unlock();
  fPreResponseSemaphore.Wait();
  fIAgent->Lock();
  if (!fIAgent->SignalResponse(pRangeQueryMessage->GetIdentifier(),
      pRangeQueryMessage->GetOriginalAlp().GetId())) {
    LOG(logWARNING)
    << "IAlp::ProcessRangeQueryMessage(" << GetRank()
        << ")# Ignoring message with unsuccessful signal response: "
        << *pRangeQueryMessage;
    fIAgent->Unlock();
    return;
  }
  fIAgent->Unlock();
  fReceiveProcessSemaphore.Wait();
  fIAgent->Lock();
  fIAgent->SignalPostReceive();
  fIAgent->Unlock();
}

bool IAlp::ProcessRollback(const RollbackMessage* pRollbackMessage) {
  // Check if agent ID has been stored in LVT map
  if (!fIAgent->HasAgentID(pRollbackMessage->GetOriginalAlp().GetId())) {
    LOG(logERROR)
    << "IAlp::ProcessRollback(" << GetRank()
        << ")# Rollback message received for unknown agent ID, ignore rollback, agent ID: "
        << pRollbackMessage->GetOriginalAlp().GetId() << ", rollback message: "
        << *pRollbackMessage;
    return false;
  }
  // Check if rollback already on tag list
  if (CheckRollbackTagList(pRollbackMessage->GetRollbackTag())) {
    LOG(logFINEST)
    << "IAlp::ProcessRollback(" << GetRank()
        << ")# Rollback message tag already on list, ignore rollback: "
        << *pRollbackMessage;
    return false;
  }
  // Check if rollback rolls back far enough
  if (pRollbackMessage->GetTimestamp() > fIAgent->GetAgentLVT(pRollbackMessage->GetOriginalAlp().GetId())) {
    return false;
  }
  // Rollback message is good, rollback

  // Reset LVT map
  LOG(logFINEST) << "IAlp::ProcessRollback(" << GetRank() << ")# Rollback agent : " << pRollbackMessage->GetOriginalAlp().GetId() << ", to LVT: " << pRollbackMessage->GetTimestamp();
  fIAgent->RollbackAgentLVT(pRollbackMessage->GetOriginalAlp().GetId(), pRollbackMessage->GetTimestamp());

  /*
   We first need to remove all those events with time stamp greater than
   that of the roll-back which have been generated and not yet sent.  i.e.,
   remove all messages in the sendQ which have time greater than roll-back
   time. We do this before generating the anti-messages to prevent them from
   also being deleted from the sendQ.
   */
  fSendMessageQueue->RemoveMessages(pRollbackMessage->GetOriginalAlp(), pRollbackMessage->GetTimestamp());
  /*
   Next stage of roll-back is to process the sentList, sending out
   anti-messages for all the messages that have been sent out by this
   Alp after the time of the roll-back.
   */
  list<SharedStateMessage*> rollbackMessagesInSendList = RollbackSendList( pRollbackMessage->GetTimestamp(), pRollbackMessage->GetOriginalAlp());
  for (list<SharedStateMessage*>::iterator iter = rollbackMessagesInSendList.begin(); iter != rollbackMessagesInSendList.end();) {
    if (fGVT > (*iter)->GetTimestamp()) {
      LOG(logWARNING)
      << "IAlp::ProcessRollback(" << GetRank()
          << ")# Rolling back before gvt: " << fGVT << " Message Time: "
          << (*iter)->GetTimestamp() << " Message: " << *iter;
    } else {
      switch ((*iter)->GetType()) {
        case SINGLEREADMESSAGE : {
          SingleReadMessage* singleReadMessage = static_cast<SingleReadMessage*> (*iter);
          SingleReadAntiMessage* antiSingleReadMessage = new SingleReadAntiMessage();
          antiSingleReadMessage->SetOrigin(GetRank());
          antiSingleReadMessage->SetDestination(GetParentClp());
          antiSingleReadMessage->SetTimestamp(singleReadMessage->GetTimestamp());
          // Mattern colour?
          antiSingleReadMessage->SetNumberOfHops(0);
          antiSingleReadMessage->SetRollbackTag(pRollbackMessage->GetRollbackTag());
          antiSingleReadMessage->SetOriginalAlp(singleReadMessage->GetOriginalAlp());
          antiSingleReadMessage->SetSsvId(singleReadMessage->GetSsvId());
          antiSingleReadMessage->Send(this);
        }
          break;
        case WRITEMESSAGE : {
          WriteMessage* writeMessage = static_cast<WriteMessage*> (*iter);
          WriteAntiMessage* antiWriteMessage = new WriteAntiMessage();
          antiWriteMessage->SetOrigin(GetRank());
          antiWriteMessage->SetDestination(GetParentClp());
          antiWriteMessage->SetTimestamp(writeMessage->GetTimestamp());
          // Mattern colour?
          antiWriteMessage->SetNumberOfHops(0);
          antiWriteMessage->SetRollbackTag(pRollbackMessage->GetRollbackTag());
          antiWriteMessage->SetOriginalAlp(writeMessage->GetOriginalAlp());
          antiWriteMessage->SetSsvId(writeMessage->GetSsvId());
          antiWriteMessage->Send(this);
        }
          break;
        case RANGEQUERYMESSAGE : {
          RangeQueryMessage* rangeQueryMessage = static_cast<RangeQueryMessage*> (*iter);
          RangeQueryAntiMessage* antiRangeQueryMessage = new RangeQueryAntiMessage();
          antiRangeQueryMessage->SetOrigin(GetRank());
          antiRangeQueryMessage->SetDestination(GetParentClp());
          antiRangeQueryMessage->SetTimestamp(rangeQueryMessage->GetTimestamp());
          // Mattern colour?
          antiRangeQueryMessage->SetNumberOfHops(0);
          antiRangeQueryMessage->SetRollbackTag(pRollbackMessage->GetRollbackTag());
          antiRangeQueryMessage->SetOriginalAlp(rangeQueryMessage->GetOriginalAlp());
          antiRangeQueryMessage->SetRange(rangeQueryMessage->GetRange());
          antiRangeQueryMessage->SetIdentifier(rangeQueryMessage->GetIdentifier());
          antiRangeQueryMessage->Send(this);
        }
          break;
        default :
          LOG(logWARNING)
          << "IAlp::ProcessRollback(" << GetRank()
              << ")# Unsuitable message found to create antimessage from: "
              << *iter;
          break;
      }
    }
    // I don't need to free the memory for value for WriteMessages as that was freed by SendThread when the message
    // was send originally. It was never stored in the send-list anyway.
    delete *iter;
    rollbackMessagesInSendList.erase(iter++);
  }
  return true;
}

unsigned int IAlp::GetParentClp() const {
  return fParentClp;
}

unsigned long IAlp::GetLvt() const {
  return fIAgent->GetLVT();
}

void IAlp::SetGvt(unsigned long pGvt) {
  fGVT = pGvt;
  ClearSendList(pGvt);
  ClearRollbackTagList(pGvt);
}

void IAlp::SignalReceiveProcess() {
  fReceiveProcessSemaphore.Signal();
}

void IAlp::SignalOutsideMessage() {
  fOutsideMessageSemaphore.Signal();
}

void IAlp::SignalPreResponse() {
  fPreResponseSemaphore.Signal();
}

void IAlp::SetIgnoreID(unsigned long pIgnoreID) {
  fIgnoreIDList.push_back(pIgnoreID);
}

bool IAlp::CheckIgnoreID(unsigned long pIgnoreID) {
  list<unsigned long>::iterator iter = find(fIgnoreIDList.begin(),
      fIgnoreIDList.end(), pIgnoreID);
  if (iter != fIgnoreIDList.end()) {
    fIgnoreIDList.erase(iter);
    return true;
  }
  return false;
}

void IAlp::Initialise() {
  // Nothing yet
}

void IAlp::Finalise() {
  fMPIInterface->StopSimulation();
  if (fIAgent->HasResponseMessageWaiting()) fIAgent->SignalResponse();
  fMPIInterface->Join();
}
