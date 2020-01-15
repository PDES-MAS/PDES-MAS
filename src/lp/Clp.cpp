#include "Clp.h"
#include "Value.h"
#include "SingleReadResponseMessage.h"
#include "WriteAntiMessage.h"
#include "SingleReadAntiMessage.h"
#include "LoadBalancingMessage.h"
#include "StateMigrationMessage.h"
#include "WriteResponseMessage.h"
#include "assert.h"
#include "Log.h"
#include "Helper.h"
#include "Initialisor.h"
#include "GvtRequestMessage.h"

using Helper::string_cast;
using namespace pdesmas;
using namespace std;

Clp::Clp(unsigned int pRank, unsigned int pCommSize,
         unsigned int pNumberOfClps, unsigned int pNumberOfAlps,
         unsigned long pStartTime, unsigned long pEndTime,
         const Initialisor *initialisor) {
  SetRank(pRank);
  SetSize(pCommSize);
  SetNumberOfClps(pNumberOfClps);
  SetNumberOfAlps(pNumberOfAlps);
  fStartTime = pStartTime;
  fEndTime = pEndTime;
  fGVT = fStartTime;

  fEndMessageProcessed = false;

  fEndMessagesReceived = new bool[GetNumberOfAlps()];
  for (unsigned int i = 0; i < GetNumberOfAlps(); ++i) {
    fEndMessagesReceived[i] = false;
  }

  fGVTCalculator = new GvtCalculator(this);

#ifdef SSV_LOCALISATION
  fAccessCostCalculator
      = new AccessCostCalculator(GetRank(), GetNumberOfClps());
  fSharedState.SetAccessCostCalculator(fAccessCostCalculator);
  fStopLoadBalanceProcessing = false;
#endif

  auto ssv_id_value_map = initialisor->GetClpSsvIdValueMap(); // insert initial values
  auto clp_id_ssv_id_map = initialisor->GetClpToSsvMap();
  auto ssv_it = clp_id_ssv_id_map.find(this->GetRank());
  if (ssv_it != clp_id_ssv_id_map.end()) {
    auto my_ssv_list = ssv_it->second;
    for (auto &iter : my_ssv_list) {
      this->AddSSV(iter, ssv_id_value_map.find(iter)->second);
    }
  }

  fRouter = new Router(GetRank(), GetNumberOfClps(), initialisor);

  fMPIInterface = new MpiInterface(this, this);

#ifdef RANGE_QUERIES
  fRangeRoutingTable = vector<RangeRoutingTable *>(DIRECTION_SIZE);
  for (int ports = 0; ports < DIRECTION_SIZE; ++ports)
    fRangeRoutingTable[ports] = new RangeRoutingTable();
  fSharedState.SetRangeRoutingTable(fRangeRoutingTable[0]);
  fRangeTracker = new RangeTracker();
  InitialisePortRanges(initialisor);
#endif

  // Wait for all CLPs and ALPs to come online
  MPI_Barrier(MPI_COMM_WORLD);
  // Initialise the MPI connections
  fMPIInterface->Signal();
  // Wait for all MPI connections to come online
  MPI_Barrier(MPI_COMM_WORLD);
}

void Clp::AddSSV(const SsvId &pSSVID, const AbstractValue *pValue) {
  /*
   * I initialise the first write period of the SSV with (0,0) mostly because I don't
   * know yet where the SSV is located. This information is not included in the
   * initialisation file. I should really provide this information there and properly
   * initialise the first write period of an SSV.
   */
  // TODO Fix initialisation of the first write period
  fSharedState.Add(pSSVID, pValue, fStartTime, LpId(0, 0));
}

void Clp::SetGvt(unsigned long pGVT) {
  // Set GVT
  fGVT = pGVT;
  // Remove write periods before GVT
  spdlog::debug("Clp {}, SetGvt({}), remove write periods", GetRank(), fGVT);
  fSharedState.RemoveWritePeriods(fGVT);
#ifdef RANGE_QUERIES
  // Clear range periods
  for (int ports = 0; ports < DIRECTION_SIZE; ports++)
    fRangeRoutingTable[ports]->ClearRangePeriods(fGVT);
#endif
  // If end message has been processed
  if (fEndMessageProcessed) {
    // Reset end message processed
    fEndMessageProcessed = false;
#ifdef SSV_LOCALISATION
    // Stop load balancing
    fStopLoadBalanceProcessing = true;
#endif
  } else {
#ifdef SSV_LOCALISATION
    // Restart load balancing
    fStopLoadBalanceProcessing = false;
    // If CLP load is sufficient
    if (fAccessCostCalculator->CheckClpLoad()) {
      // Trigger state migration
      MigrateStateVariables(fAccessCostCalculator->GetMigrationMap());
    }
#endif
  }
}

void Clp::Send() {
  AbstractMessage *sendMessage = NULL;
  if (!fSendLoadBalancingMessageQueue->IsEmpty()) {
    sendMessage = fSendLoadBalancingMessageQueue->DequeueMessage();
  } else if (!fSendControlMessageQueue->IsEmpty()) {
    sendMessage = fSendControlMessageQueue->DequeueMessage();
  } else if (!fSendMessageQueue->IsEmpty()) {
    sendMessage = fSendMessageQueue->DequeueMessage();
  }
  if (NULL == sendMessage) return;

  switch (sendMessage->GetType()) {
    case SINGLEREADMESSAGE :
      PreProcessSendMessage(static_cast<SingleReadMessage *> (sendMessage));
      break;
    case SINGLEREADRESPONSEMESSAGE : {
      SingleReadResponseMessage *singleReadResponseMessage = static_cast<SingleReadResponseMessage *>(sendMessage);
      // TODO: Check if reroute is necessary
      fRouter->Route(singleReadResponseMessage);
      PreProcessSendMessage(singleReadResponseMessage);
    }
      break;
    case SINGLEREADANTIMESSAGE :
      PreProcessSendMessage(static_cast<SingleReadAntiMessage *> (sendMessage));
      break;
    case WRITEMESSAGE :
      PreProcessSendMessage(static_cast<WriteMessage *> (sendMessage));
      break;
    case WRITERESPONSEMESSAGE : {
      WriteResponseMessage *writeResponseMessage = static_cast<WriteResponseMessage *>(sendMessage);
      // TODO: Check if reroute is necessary
      fRouter->Route(writeResponseMessage);
      PreProcessSendMessage(writeResponseMessage);
    }
      break;
    case WRITEANTIMESSAGE :
      PreProcessSendMessage(static_cast<WriteAntiMessage *> (sendMessage));
      break;
    case RANGEQUERYMESSAGE :
      PreProcessSendMessage(static_cast<RangeQueryMessage *> (sendMessage));
      break;
    case ROLLBACKMESSAGE : {
      RollbackMessage *rollbackMessage = static_cast<RollbackMessage *>(sendMessage);
      // TODO: Check if reroute is necessary
      fRouter->Route(rollbackMessage);
      PreProcessSendMessage(rollbackMessage);
    }
      break;
    case STATEMIGRATIONMESSAGE :
      // TODO: Check if reroute is necessary
      fRouter->Route(static_cast<StateMigrationMessage *>(sendMessage));
      break;
    default :
      // Skip
      break;
  }
  fMPIInterface->Send(sendMessage);
}

void Clp::Receive() {
  // Declare received message
  AbstractMessage *receivedMessage = NULL;
  if (!fReceiveLoadBalancingMessageQueue->IsEmpty()) {
    // If there's a load balancing message, deal with that first
    receivedMessage = fReceiveLoadBalancingMessageQueue->DequeueMessage();
  } else if (!fReceiveMessageQueue->IsEmpty()) {
    // If there's a message, deal with it
    receivedMessage = fReceiveMessageQueue->DequeueMessage();
  }
  // If there's no message, return
  if (NULL == receivedMessage) return;

  // If the received message is a simulation message, increase #hops
  switch (receivedMessage->GetType()) {
    case SINGLEREADMESSAGE: {
      SingleReadMessage *singleReadMessage =
          static_cast<SingleReadMessage *> (receivedMessage);
      singleReadMessage->IncrementNumberOfHops();
      PreProcessReceiveMessage(singleReadMessage);
      if (fRouter->Route(singleReadMessage)) {
        ProcessMessage(singleReadMessage);
        PostProcessMessage();
        delete singleReadMessage;
      } else singleReadMessage->SendToLp(this);
    }
      break;
    case SINGLEREADRESPONSEMESSAGE: {
      SingleReadResponseMessage *singleReadResponseMessage =
          static_cast<SingleReadResponseMessage *> (receivedMessage);
      PreProcessReceiveMessage(singleReadResponseMessage);
      if (fRouter->Route(singleReadResponseMessage)) {
        LOG(logERROR)
          << "Clp::Receive(" << GetRank()
          << ")# SingleReadResponseMessage received for this CLP while CLPs don't handle response messages! "
          << *receivedMessage;
      } else singleReadResponseMessage->SendToLp(this);
    }
      break;
    case SINGLEREADANTIMESSAGE: {
      SingleReadAntiMessage *singleReadAntiMessage =
          static_cast<SingleReadAntiMessage *> (receivedMessage);
      singleReadAntiMessage->IncrementNumberOfHops();
      PreProcessReceiveMessage(singleReadAntiMessage);
      if (fRouter->Route(singleReadAntiMessage)) {
        ProcessMessage(singleReadAntiMessage);
        PostProcessMessage();
        delete singleReadAntiMessage;
      } else singleReadAntiMessage->SendToLp(this);
    }
      break;
    case WRITEMESSAGE: {
      WriteMessage *writeMessage = static_cast<WriteMessage *> (receivedMessage);
      writeMessage->IncrementNumberOfHops();
      PreProcessReceiveMessage(writeMessage);
      if (fRouter->Route(writeMessage)) {
        ProcessMessage(writeMessage);
        PostProcessMessage();
        writeMessage->ClearValue();
        delete writeMessage;
      } else writeMessage->SendToLp(this);
    }
      break;
    case WRITERESPONSEMESSAGE: {
      WriteResponseMessage *writeResponseMessage =
          static_cast<WriteResponseMessage *> (receivedMessage);
      PreProcessReceiveMessage(writeResponseMessage);
      if (fRouter->Route(writeResponseMessage)) {
        LOG(logERROR)
          << "Clp::Receive(" << GetRank()
          << ")# WriteResponseMessage received for this CLP while CLPs don't handle response messages! "
          << *receivedMessage;
      } else writeResponseMessage->SendToLp(this);
    }
      break;
    case WRITEANTIMESSAGE: {
      WriteAntiMessage *writeAntiMessage =
          static_cast<WriteAntiMessage *> (receivedMessage);
      writeAntiMessage->IncrementNumberOfHops();
      PreProcessReceiveMessage(writeAntiMessage);
      if (fRouter->Route(writeAntiMessage)) {
        ProcessMessage(writeAntiMessage);
        PostProcessMessage();
        delete writeAntiMessage;
      } else writeAntiMessage->SendToLp(this);
    }
      break;
#ifdef RANGE_QUERIES
    case RANGEQUERYMESSAGE: {
      RangeQueryMessage *rangeQueryMessage =
          static_cast<RangeQueryMessage *> (receivedMessage);
      rangeQueryMessage->IncrementNumberOfHops();
      PreProcessReceiveMessage(rangeQueryMessage);
      ProcessMessage(rangeQueryMessage);
      PostProcessMessage();
      delete rangeQueryMessage;
    }
      break;
    case RANGEQUERYANTIMESSAGE: {
      RangeQueryAntiMessage *rangeQueryAntiMessage =
          static_cast<RangeQueryAntiMessage *> (receivedMessage);
      rangeQueryAntiMessage->IncrementNumberOfHops();
      PreProcessReceiveMessage(rangeQueryAntiMessage);
      ProcessMessage(rangeQueryAntiMessage);
      PostProcessMessage();
      delete rangeQueryAntiMessage;
    }
      break;
    case RANGEUPDATEMESSAGE: {
      RangeUpdateMessage *rangeUpdateMessage =
          static_cast<RangeUpdateMessage *> (receivedMessage);
      ProcessMessage(rangeUpdateMessage);
      PostProcessMessage();
      delete rangeUpdateMessage;
    }
      break;
#endif
    case ROLLBACKMESSAGE: {
      RollbackMessage *rollbackMessage =
          static_cast<RollbackMessage *> (receivedMessage);
      PreProcessReceiveMessage(rollbackMessage);
      if (fRouter->Route(rollbackMessage)) {
        LOG(logERROR)
          << "Clp::Receive(" << GetRank()
          << ")# RollbackMessage received for this CLP while CLPs don't handle response messages! "
          << *receivedMessage;
      } else rollbackMessage->SendToLp(this);
    }
      break;
    case ENDMESSAGE: {
      EndMessage *endMessage = static_cast<EndMessage *> (receivedMessage);
      ProcessMessage(endMessage);
      PostProcessMessage();
      delete endMessage;
    }
      break;
    case GVTCONTROLMESSAGE: {
      GvtControlMessage *gvtControlMessage =
          static_cast<GvtControlMessage *> (receivedMessage);
      fGVTCalculator->ProcessMessage(gvtControlMessage);
      delete gvtControlMessage;
    }
      break;
    case GVTREQUESTMESSAGE: {
      GvtRequestMessage *gvtRequestMessage =
          static_cast<GvtRequestMessage *> (receivedMessage);
      fGVTCalculator->ProcessMessage(gvtRequestMessage);
      delete gvtRequestMessage;
    }
      break;
    case GVTVALUEMESSAGE: {
      GvtValueMessage *gvtValueMessage =
          static_cast<GvtValueMessage *> (receivedMessage);
      fGVTCalculator->ProcessMessage(gvtValueMessage);
      delete gvtValueMessage;
    }
      break;
#ifdef SSV_LOCALISATION
    case STATEMIGRATIONMESSAGE: {
      StateMigrationMessage *stateMigrationMessage =
          static_cast<StateMigrationMessage *> (receivedMessage);
      if (fRouter->Route(stateMigrationMessage)) {
        ProcessMessage(stateMigrationMessage);
        PostProcessMessage();
        delete stateMigrationMessage;
      } else stateMigrationMessage->SendToLp(this);
    }
      break;
#endif
    default:
      LOG(logERROR)
        << "Clp::Receive(" << GetRank() << ")# Received inappropriate message: "
        << *receivedMessage;
      exit(1);
  }
}

unsigned long Clp::GetLvt() const {
  return ULONG_MAX;
}

void Clp::ProcessMessage(const SingleReadMessage *pSingleReadMessage) {
  // SendReadMessageAndGetResponse the value of the SSV
  LOG(logFINEST) << "Clp::ProcessMessage(SingleReadMessage)(" << GetRank()
                 << ")# SendReadMessageAndGetResponse SSV from message: " << *pSingleReadMessage;
  AbstractValue *value = fSharedState.Read(pSingleReadMessage->GetSsvId(),
                                           pSingleReadMessage->GetOriginalAgent(), pSingleReadMessage->GetTimestamp());
  // Create and send response message
  SingleReadResponseMessage *singleReadMessageResponse =
      new SingleReadResponseMessage();
  singleReadMessageResponse->SetOrigin(GetRank());
  singleReadMessageResponse->SetDestination(
      pSingleReadMessage->GetOriginalAgent().GetRank());
  singleReadMessageResponse->SetTimestamp(pSingleReadMessage->GetTimestamp());
  // mattern colour set by GVT Calculator
  singleReadMessageResponse->SetIdentifier(pSingleReadMessage->GetIdentifier());
  singleReadMessageResponse->SetOriginalAgent(
      pSingleReadMessage->GetOriginalAgent());
  // Value is cloned in the message
  singleReadMessageResponse->SetValue(value);
  singleReadMessageResponse->SendToLp(this);
#ifdef SSV_LOCALISATION
  // Update access count for state migration
  fSharedState.UpdateAccessCount(
      pSingleReadMessage->GetSsvId(),
      fRouter->GetDirectionByLpRank(
          pSingleReadMessage->GetOriginalAgent().GetRank()),
      pSingleReadMessage->GetNumberOfHops());
#endif
}

void Clp::ProcessMessage(const SingleReadAntiMessage *pSingleReadAntiMessage) {
  // Check for rollbacks to before GVT
  if (fGVT > pSingleReadAntiMessage->GetTimestamp()) {
    ostringstream out;
    (*pSingleReadAntiMessage).Serialise(out);
    spdlog::error("Clp::ProcessMessage(SingleReadAntiMessage)({}) Trying to rollback to before GVT({}): {}", GetRank(),
                  fGVT, out.str());

    return;
  }
#ifdef RANGE_QUERIES
  // Clear range updates
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
  // Declare rollbacklist
  RollbackList rollbacklist;
  // If anti read message, rollback read
  fSharedState.RollbackRead(pSingleReadAntiMessage->GetSsvId(),
                            pSingleReadAntiMessage->GetOriginalAgent(),
                            pSingleReadAntiMessage->GetTimestamp());
  // Send rollbacks if necessary
  rollbacklist.SendRollbacks(this, pSingleReadAntiMessage->GetRollbackTag());
#ifdef RANGE_QUERIES
  // Send range updates
  SendRangeUpdates(fRangeRoutingTable[HERE]->GetRangeUpdateList(), HERE);
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
#ifdef SSV_LOCALISATION
  // Update access count
  fSharedState.UpdateAccessCount(
      pSingleReadAntiMessage->GetSsvId(),
      fRouter->GetDirectionByLpRank(
          pSingleReadAntiMessage->GetOriginalAgent().GetRank()),
      pSingleReadAntiMessage->GetNumberOfHops());
#endif
}

void Clp::ProcessMessage(const WriteMessage *pWriteMessage) {
#ifdef RANGE_QUERIES
  // Clear range updates
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
  // Declare write status and rollbacklist variables
  WriteStatus writeStatus;
  RollbackList rollbacklist;
  // Write the value with rollback
  fSharedState.WriteWithRollback(pWriteMessage->GetSsvId(),
                                 pWriteMessage->GetOriginalAgent(), pWriteMessage->GetValue(),
                                 pWriteMessage->GetTimestamp(), writeStatus, rollbacklist);
  // Create write message response and send it
  WriteResponseMessage *writeMessageResponse = new WriteResponseMessage();
  writeMessageResponse->SetOrigin(GetRank());
  writeMessageResponse->SetDestination(
      pWriteMessage->GetOriginalAgent().GetRank());
  writeMessageResponse->SetTimestamp(pWriteMessage->GetTimestamp());
  // Mattern colour set by GVT Calculator
  writeMessageResponse->SetIdentifier(pWriteMessage->GetIdentifier());
  writeMessageResponse->SetOriginalAgent(pWriteMessage->GetOriginalAgent());
  writeMessageResponse->SetWriteStatus(writeStatus);
  writeMessageResponse->SendToLp(this);
  // If there are rollback messages to send, do so
  if (rollbacklist.GetSize() > 0) {
    RollbackTag rollbackTag(pWriteMessage->GetSsvId(),
                            pWriteMessage->GetTimestamp(), ROLLBACK_BY_WRITE);
    rollbacklist.SendRollbacks(this, rollbackTag);
  }
#ifdef RANGE_QUERIES
  // Send range updates and clear table
  SendRangeUpdates(fRangeRoutingTable[HERE]->GetRangeUpdateList(), HERE);
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
#ifdef SSV_LOCALISATION
  // Update the access count for state migration
  fSharedState.UpdateAccessCount(pWriteMessage->GetSsvId(),
                                 fRouter->GetDirectionByLpRank(pWriteMessage->GetOriginalAgent().GetRank()),
                                 pWriteMessage->GetNumberOfHops());
#endif
}

void Clp::ProcessMessage(const WriteAntiMessage *pWriteAntiMessage) {
  // Check for rollbacks to before GVT
  if (fGVT > pWriteAntiMessage->GetTimestamp()) {
    LOG(logERROR) << "Clp::ProcessMessage(WriteAntiMessage)(" << GetRank() << ")# Trying to rollback to before GVT: "
                  << fGVT << ", message: " << *pWriteAntiMessage;
    return;
  }
#ifdef RANGE_QUERIES
  // Clear range updates
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
  // Declare rollbacklist
  RollbackList rollbacklist;
  // If anti write message, rollback write
  fSharedState.RollbackWrite(pWriteAntiMessage->GetSsvId(),
                             pWriteAntiMessage->GetOriginalAgent(), pWriteAntiMessage->GetTimestamp(),
                             rollbacklist);
  // Send rollbacks if necessary
  rollbacklist.SendRollbacks(this, pWriteAntiMessage->GetRollbackTag());
#ifdef RANGE_QUERIES
  // Send range updates
  SendRangeUpdates(fRangeRoutingTable[HERE]->GetRangeUpdateList(), HERE);
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
#ifdef SSV_LOCALISATION
  // Update access count
  fSharedState.UpdateAccessCount(
      pWriteAntiMessage->GetSsvId(),
      fRouter->GetDirectionByLpRank(
          pWriteAntiMessage->GetOriginalAgent().GetRank()),
      pWriteAntiMessage->GetNumberOfHops());
#endif
}

void Clp::ProcessMessage(const EndMessage *pEndMessage) {
  unsigned int parentCLP = (((int) ((GetRank() + 1) / 2)) - 1);
  unsigned int leftCLP = 2 * GetRank() + 1;
  unsigned int rightCLP = 2 * GetRank() + 2;

#ifdef SSV_LOCALISATION
  fStopLoadBalanceProcessing = true;
#endif
  EndMessage *oneEndMessage = new EndMessage;
  *oneEndMessage = *pEndMessage;
  if ((GetRank() != 0) && (GetRank() < GetNumberOfClps() / 2)) {
    // If we're a CLP in the middle
    fEndMessageProcessed = true;
    EndMessage *twoEndMessage = new EndMessage;
    *twoEndMessage = *oneEndMessage;
    twoEndMessage->SetDestination(parentCLP);
    if (oneEndMessage->GetOrigin() == leftCLP) {
      oneEndMessage->SetDestination(rightCLP);
    } else if (oneEndMessage->GetOrigin() == rightCLP) {
      oneEndMessage->SetDestination(leftCLP);
    } else if (oneEndMessage->GetOrigin() == parentCLP) {
      oneEndMessage->SetDestination(leftCLP);
      twoEndMessage->SetDestination(rightCLP);
    } else {
      LOG(logWARNING)
        << "Clp::ProcessEndMessage# Routing EndMessage with unknown origin!";
    }
    oneEndMessage->SetOrigin(GetRank());
    twoEndMessage->SetOrigin(GetRank());
    oneEndMessage->SendToLp(this);
    twoEndMessage->SendToLp(this);
  } else if ((GetRank() != 0) && (GetRank() >= GetNumberOfClps() / 2)) {
    // If we're a parent CLP
    fEndMessageProcessed = true;
    if (oneEndMessage->GetOrigin() != parentCLP) {
      oneEndMessage->SetDestination(parentCLP);
      oneEndMessage->SetOrigin(GetRank());
      oneEndMessage->SendToLp(this);
    }
  } else {
    // We must be the root CLP
    fEndMessagesReceived[oneEndMessage->GetSenderAlp() - GetNumberOfClps()]
        = true;
    if (AllEndMessagesReceived()) {
      if (!fEndMessageProcessed) {
        GvtRequestMessage *gvtRequestMessage = new GvtRequestMessage;
        gvtRequestMessage->SetOrigin(GetRank());
        gvtRequestMessage->SetDestination(GetRank());
        fGVTCalculator->ProcessMessage(gvtRequestMessage);
        delete gvtRequestMessage;
      }
      fEndMessageProcessed = true;
    }
    // But if the root is not a parent, then I need to forward the messages
    if (GetRank() < GetNumberOfClps() / 2) {
      if (oneEndMessage->GetOrigin() == leftCLP) {
        oneEndMessage->SetDestination(rightCLP);
      } else {
        oneEndMessage->SetDestination(leftCLP);
      }
      oneEndMessage->SetOrigin(GetRank());
      oneEndMessage->SendToLp(this);
    }
  }
}

void Clp::PostProcessMessage() {
#ifdef SSV_LOCALISATION
  /*
   if (!fStopLoadBalanceProcessing) {
   LOG(logINFO) << "Clp::PostProcessMessage# Check CLP load!";
   if (fAccessCostCalculator->CheckClpLoad()) {
   // CHECK: Migrate variables?
   if (fGVTCalculator->GetStarter()) {
   fGVTCalculator->StartGvt(this);
   } else {
   GvtRequestMessage* gvtRequestMessage = new GvtRequestMessage(GetLpId(), LpId(0), unsigned long(LONG_MAX));
   gvtRequestMessage->Send(this);
   }
   fStopLoadBalanceProcessing = true;
   }
   }
   */
#endif
}

void Clp::Initialise() {
  //nothing yet
}

void Clp::Finalise() {
  fMPIInterface->StopSimulation();
  fMPIInterface->Join();
}

#ifdef SSV_LOCALISATION

void Clp::ProcessMessage(const StateMigrationMessage *pStateMigrationMessage) {
#ifdef RANGE_QUERIES
  // Clear range updates
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
  // Get SSVID state variable map
  SerialisableMap<SsvId, StateVariable> stateVariableMap = pStateMigrationMessage->GetStateVariableMap();
  SerialisableMap<SsvId, StateVariable>::iterator stateVariableMapIterator = stateVariableMap.begin();
  // For each state variable in the map
  while (stateVariableMapIterator != stateVariableMap.end()) {
    // Set the direction of the incoming SSV to the current CLP
    fRouter->SetSsvIdDirection(stateVariableMapIterator->first, (Direction) HERE);
    // Insert the state variable
    RollbackList rollbackList;
    fSharedState.Insert(stateVariableMapIterator->first, stateVariableMapIterator->second, rollbackList);
    // Send rollbacks if necessary
    if (rollbackList.GetSize() > 0) {
      RollbackTag rbTag(stateVariableMapIterator->first, ULONG_MAX, ROLLBACK_BY_SM);
      rollbackList.SendRollbacks(this, rbTag);
    }
#ifdef RANGE_QUERIES
    // Send range updates
    SendRangeUpdates(fRangeRoutingTable[HERE]->GetRangeUpdateList(), HERE);
    fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
    // Move on to the next SSV
    ++stateVariableMapIterator;
  }
}

void Clp::MigrateStateVariables(
    const map<Direction, list<SsvId> > &pMigrationMap) {
  spdlog::warn("SV Migration!");
  for (auto &i : pMigrationMap) {
    for (auto &i2:i.second) {
      spdlog::warn("{0}, to {1}", i2.id(), i.first);
    }
  }
  // Get the list of SSVs to migrate to which port
  map<Direction, list<SsvId> >::const_iterator migrateSSVMapIterator = pMigrationMap.begin();

  // For each direction in the list
  while (migrateSSVMapIterator != pMigrationMap.end()) {
    // Create a new load balancing load message
    StateMigrationMessage *loadBalancingLoadMessage = new StateMigrationMessage();
    loadBalancingLoadMessage->SetOrigin(GetRank());
    loadBalancingLoadMessage->SetDestination(fRouter->GetLpRankByDirection(migrateSSVMapIterator->first));
#ifdef RANGE_QUERIES
    // Clear the range updates in the routing table
    fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
    // For each SSV in the list
    list<SsvId>::const_iterator ssvIdListIterator = (migrateSSVMapIterator->second).begin();
    while (ssvIdListIterator != (migrateSSVMapIterator->second).end()) {
      // Set the state variable in the load balancing load message
      loadBalancingLoadMessage->SetStateVariableMap(*ssvIdListIterator, fSharedState.GetCopy(*ssvIdListIterator));
      // Update the direction for this SSV
      fRouter->SetSsvIdDirection(*ssvIdListIterator, migrateSSVMapIterator->first);
      // Remove all write periods
      RollbackList rollbackList;
      fSharedState.RemoveWritePeriodList(*ssvIdListIterator, rollbackList);
      // Delete the SSV from the shared state
      fSharedState.Delete(*ssvIdListIterator);
      // Send rollbacls if necessary
      if (rollbackList.GetSize() > 0) {
        RollbackTag rbTag(*ssvIdListIterator, ULONG_MAX, ROLLBACK_BY_SM);
        LOG(logFINEST) << "Sending rollbacks after state migration";
        rollbackList.SendRollbacksAfterStateMigration(this, rbTag);
      }
#ifdef RANGE_QUERIES
      // Send range updates
      SendRangeUpdates(fRangeRoutingTable[HERE]->GetRangeUpdateList(), HERE);
      fRangeRoutingTable[HERE]->ClearRangeUpdates();
#endif
      // Move on to next SSV for this direction
      ++ssvIdListIterator;
    }
    // Send load balancing load message
    loadBalancingLoadMessage->SendToLp(this);
    // Move on to next direction
    ++migrateSSVMapIterator;
  }
}

#endif

#ifdef RANGE_QUERIES

void Clp::ProcessMessage(const RangeQueryMessage *pRangeQueryMessage) {
  RangeQueryMessage *newRangeQueryMessage = new RangeQueryMessage();
  *newRangeQueryMessage = *pRangeQueryMessage;
  // Declare block state map
  map<Direction, BlockStatus> blockStatus;
  // Increase TR hops
  newRangeQueryMessage->IncrementNumberOfTraverseHops();
  // If this CLP hasn't been seen before
  if (!(fRangeTracker->HasEntry(newRangeQueryMessage->GetIdentifier()))) {
    // This CLP hasn't been seen before
    // Find the incoming port
    Direction incomingPort = fRouter->GetDirectionByLpRank(
        newRangeQueryMessage->GetOriginalAgent().GetRank());
    // Add an entry for this RQ in the range tracker
    fRangeTracker->CreateEntry(newRangeQueryMessage->GetIdentifier(),
                               newRangeQueryMessage->GetSsvValueList(), incomingPort,
                               newRangeQueryMessage->GetNumberOfHops());
    // Clear SSV list of RQ message
    newRangeQueryMessage->ClearSsvList();
    // Reset the number of hops
    newRangeQueryMessage->SetNumberOfHops(0);
    // Mark the incoming for AVOID
    blockStatus.insert(make_pair(incomingPort, AVOID));
    // Declare a new range query message
    RangeQueryMessage *tempRangeQueryMessage = NULL;
    // Get the outstanding ports and get an iterator to the list
    list<Direction> outstandingPortList = fRangeTracker->GetOutstandingPorts(
        newRangeQueryMessage->GetIdentifier());
    list<Direction>::iterator portListIterator = outstandingPortList.begin();
    while (portListIterator != outstandingPortList.end()) {
      // Port is not the incoming port, a port to an ALP, or the up port in the root
      if ((*portListIterator == incomingPort) || ((GetRank()
                                                   >= (GetNumberOfClps() / 2)) && (*portListIterator == LEFT
                                                                                   || *portListIterator == RIGHT)) ||
          ((GetRank() == 0)
           && (*portListIterator == UP))) {
        // This port is not a target port, just copy the message
        tempRangeQueryMessage = newRangeQueryMessage;
      } else {
        // Port is a target port, forward the range query through the tree
        if (!fRangeRoutingTable[(int) *portListIterator]->HasRangePeriod(
            newRangeQueryMessage->GetTimestamp())) {
          tempRangeQueryMessage = NULL;
        } else {
          Range *range =
              fRangeRoutingTable[(int) *portListIterator]->GetRangeCopy(
                  newRangeQueryMessage->GetTimestamp());
          if (range != NULL && range->IsRangeOverlapping(
              newRangeQueryMessage->GetRange())) {
            fRangeRoutingTable[(int) *portListIterator]->AddRangeQueryInfo(
                newRangeQueryMessage->GetTimestamp(),
                newRangeQueryMessage->GetIdentifier(),
                newRangeQueryMessage->GetRange(), false,
                newRangeQueryMessage->GetOriginalAgent());
            blockStatus.insert(make_pair(*portListIterator, NOT_BLOCKED));
            if (*portListIterator == HERE) {
              newRangeQueryMessage->SetSsvValueList(
                  fSharedState.RangeRead(
                      newRangeQueryMessage->GetRange(),
                      fRouter->GetDirectionByLpRank(
                          newRangeQueryMessage->GetOriginalAgent().GetRank()),
                      newRangeQueryMessage->GetNumberOfTraverseHops(),
                      newRangeQueryMessage->GetTimestamp()));
              tempRangeQueryMessage = newRangeQueryMessage;
            } else {
              newRangeQueryMessage->SetOrigin(GetRank());
              newRangeQueryMessage->SetDestination(
                  fRouter->GetLpRankByDirection(*portListIterator));
              RangeQueryMessage *sendRangeQueryMessage =
                  new RangeQueryMessage();
              *sendRangeQueryMessage = *newRangeQueryMessage;
              sendRangeQueryMessage->SendToLp(this);
              tempRangeQueryMessage = NULL;
            }
          } else {
            fRangeRoutingTable[(int) *portListIterator]->AddRangeQueryInfo(
                newRangeQueryMessage->GetTimestamp(),
                newRangeQueryMessage->GetIdentifier(),
                newRangeQueryMessage->GetRange(), true,
                newRangeQueryMessage->GetOriginalAgent());
            blockStatus.insert(make_pair(*portListIterator, BLOCKED));
            tempRangeQueryMessage = newRangeQueryMessage;
          }
          // Delete this copy of range if needed
          if (range) delete range;
        }
      }
      // temporary range query message is not NULL, so range query message is blocked on that port
      if (tempRangeQueryMessage != NULL) {
        // Update port as SCANNED (always true for HERE) and add the value list.
        fRangeTracker->SetPortToScanned(tempRangeQueryMessage->GetIdentifier(),
                                        *portListIterator);
        fRangeTracker->AddToSSVValueMap(tempRangeQueryMessage->GetIdentifier(),
                                        tempRangeQueryMessage->GetSsvValueList());
        // Clear the SSV list for the temporary range query message
        tempRangeQueryMessage->ClearSsvList();
      }
      // Move onto new port
      ++portListIterator;
    }
  } else {
    // This CLP has been seen before, so this range query message is a reply from a CLP
    // If there's a RQ message, process it in the range tracker
    fRangeTracker->SetPortToScanned(newRangeQueryMessage->GetIdentifier(),
                                    fRouter->GetDirectionByLpRank(newRangeQueryMessage->GetOrigin()));
    fRangeTracker->IncreaseNumberOfHops(newRangeQueryMessage->GetIdentifier(),
                                        newRangeQueryMessage->GetNumberOfHops());
    fRangeTracker->AddToSSVValueMap(newRangeQueryMessage->GetIdentifier(),
                                    newRangeQueryMessage->GetSsvValueList());
  }
  // If there are no outstanding ports all have been scanned so the response message can be send back to the original ALP
  if (fRangeTracker->GetOutstandingPorts(newRangeQueryMessage->GetIdentifier()).empty()) {
    RangeQueryMessage *rangeQueryResponse = new RangeQueryMessage();
    rangeQueryResponse->SetOrigin(GetRank());
    rangeQueryResponse->SetDestination(
        fRouter->GetLpRankByDirection(
            fRangeTracker->DeleteEntry(newRangeQueryMessage->GetIdentifier())));
    rangeQueryResponse->SetTimestamp(newRangeQueryMessage->GetTimestamp());
    // Mattern colour set by GVT Calculator
    rangeQueryResponse->SetNumberOfHops(
        fRangeTracker->GetNumberofHops(newRangeQueryMessage->GetIdentifier()));
    rangeQueryResponse->SetIdentifier(newRangeQueryMessage->GetIdentifier());
    rangeQueryResponse->SetOriginalAgent(newRangeQueryMessage->GetOriginalAgent());
    rangeQueryResponse->SetRange(newRangeQueryMessage->GetRange());
    // Block status?
    rangeQueryResponse->SetSsvValueList(
        fRangeTracker->GetCollectedSSVValueMap(
            newRangeQueryMessage->GetIdentifier()));
    // Tr hops?
    rangeQueryResponse->SendToLp(this);
  }
  delete newRangeQueryMessage;
}

void Clp::ProcessMessage(const RangeQueryAntiMessage *pRangeQueryAntiMessage) {
  // Check for rollbacks to before GVT
  if (fGVT > pRangeQueryAntiMessage->GetTimestamp()) {
    LOG(logERROR) << "Clp::ProcessMessage(RangeQueryAntiMessage)(" << GetRank()
                  << ")# Trying to rollback to before GVT: " << fGVT << ", message: " << *pRangeQueryAntiMessage;
    return;
  }
  for (int port = 0; port < DIRECTION_SIZE; ++port) {
    if ((Direction) port == fRouter->GetDirectionByLpRank(
        pRangeQueryAntiMessage->GetOriginalAgent().GetRank()))
      continue;
    if (fRangeRoutingTable[port]->HasRangePeriod(
        pRangeQueryAntiMessage->GetTimestamp())) {
      if (!fRangeRoutingTable[port]->GetBlockStatus(
          pRangeQueryAntiMessage->GetTimestamp(),
          pRangeQueryAntiMessage->GetIdentifier())) {
        if (port != HERE) {
          RangeQueryAntiMessage *sendAntiRangeQueryMessage =
              new RangeQueryAntiMessage();
          *sendAntiRangeQueryMessage = *pRangeQueryAntiMessage;
          sendAntiRangeQueryMessage->SetOrigin(GetRank());
          sendAntiRangeQueryMessage->SetDestination(
              fRouter->GetLpRankByDirection((Direction) port));
          sendAntiRangeQueryMessage->SendToLp(this);
        }
      }
      fRangeRoutingTable[port]->DeleteRangeQueryInfo(
          pRangeQueryAntiMessage->GetTimestamp(),
          pRangeQueryAntiMessage->GetIdentifier());
    }
  }
}

void Clp::ProcessMessage(const RangeUpdateMessage *pRangeUpdateMessage) {
  // Get origin port
  Direction originPort = fRouter->GetDirectionByLpRank(
      pRangeUpdateMessage->GetOrigin());
  // Get a pointer to a copy of the range
  Range *range;
  if (pRangeUpdateMessage->GetRange() != Range(Point(INT_MAX, INT_MAX),
                                               Point(INT_MAX, INT_MAX))) {
    range = new Range(pRangeUpdateMessage->GetRange());
  } else {
    range = NULL;
  }
  // Clear the range update list
  fRangeRoutingTable[(int) originPort]->ClearRangeUpdates();
  // Declare a rollback list
  RollbackList rollbacklist;
  // Update the range routing table
  fRangeRoutingTable[(int) originPort]->ProcessRangeUpdate(
      pRangeUpdateMessage->GetTimestamp(), range, rollbacklist);
  // Send rollbacks, if any
  if (rollbacklist.GetSize() > 0) {
    SsvId ssvid;
    RollbackTag rbTag(ssvid, pRangeUpdateMessage->GetTimestamp(),
                      ROLLBACK_BY_RU);
    rollbacklist.SendRollbacks(this, rbTag);
  }
  // Get the range update list from the RangeRoutingTable for that port and send it, if any
  //SendRangeUpdates(fRangeRoutingTable[(int) originPort]->GetRangeUpdateList(), originPort, rangeUpdateMessage->GetIdentifier());
  SendRangeUpdates(fRangeRoutingTable[(int) originPort]->GetRangeUpdateList(),
                   originPort);
  // Delete the range point
  if (range) delete range;
  // Clear the range updates
  fRangeRoutingTable[(int) originPort]->ClearRangeUpdates();
}

void Clp::SendRangeUpdates(const list<RangeUpdates> &pRangeUpdateList,
                           Direction pSourcePort) {
  Range *oldRange = NULL;
  Range *newRange = NULL;

  list<RangeUpdates>::const_iterator rangeUpdateListIterator =
      pRangeUpdateList.begin();
  while (rangeUpdateListIterator != pRangeUpdateList.end()) {
    // Check all ports
    for (int port = 1; port < DIRECTION_SIZE; ++port) {
      // Check if port is correct (not the source port, not an ALP port for parent CLP, and not UP for root CLP)
      if (port == pSourcePort) continue;
      if ((GetRank() >= (GetNumberOfClps() / 2)) && (port == LEFT || port
                                                                     == RIGHT))
        continue;
      if ((GetRank() == 0) && (port == UP)) continue;
      // Get copy of old and new ranges if not NULL
      oldRange = rangeUpdateListIterator->GetOldRange();
      newRange = rangeUpdateListIterator->GetNewRange();
      // This is step - 3 ****************************************
      for (int remotePort = 0; remotePort < DIRECTION_SIZE; ++remotePort) {
        // port can't be the selected port, the source port, a port to an ALP in a parent CLP, or the up port in the root CLP
        if ((remotePort != port) && (remotePort != pSourcePort) && ((GetRank()
                                                                     >= (GetNumberOfClps() / 2)) &&
                                                                    (remotePort != LEFT && remotePort
                                                                                           != RIGHT)) &&
            ((GetRank() == 0) && (remotePort != UP))) {
          // Find range information we to update, if changed send range updates
          Range *storedRange = fRangeRoutingTable[remotePort]->GetRangeCopy(
              rangeUpdateListIterator->GetTime());
          if (storedRange) {
            if (oldRange) oldRange->MinMaxofTwoRanges(*storedRange);
            else oldRange = new Range(*storedRange);

            if (newRange) newRange->MinMaxofTwoRanges(*storedRange);
            else newRange = new Range(*storedRange);
          }
          // Delete storedRage if necessary
          if (storedRange) delete storedRange;
        }
      }
      // Send range update to the selected port
      RangeUpdateMessage *rangeUpdateMessage = new RangeUpdateMessage();
      rangeUpdateMessage->SetOrigin(GetRank());
      rangeUpdateMessage->SetDestination(
          fRouter->GetLpRankByDirection((Direction) port));
      rangeUpdateMessage->SetTimestamp(rangeUpdateListIterator->GetTime());
      if ((oldRange && newRange) && (*oldRange != *newRange)) {
        // Both old and new range aren't NULL, and they differ: range update necessary
        rangeUpdateMessage->SetRange(*newRange);
      } else if (newRange) {
        // Old range is NULL, SSV created for the first time, range update necessary
        rangeUpdateMessage->SetRange(*newRange);
      } else {
        // New range is NULL, SSV are all deleted, range update to INT_MAX
        rangeUpdateMessage->SetRange(
            Range(Point(INT_MAX, INT_MAX), Point(INT_MAX, INT_MAX)));
      }
      rangeUpdateMessage->SendToLp(this);
      // This is allocated memories using AssignMinMaxofRanges function, so delete
      // it, we dont need it any more.
      if (oldRange) delete oldRange;
      if (newRange) delete newRange;
    }
    // Move on to next range update in the list
    ++rangeUpdateListIterator;
  }
}

void Clp::InitialisePortRanges(const Initialisor *pInitialisor) {
  Range *PortRange[DIRECTION_SIZE];
  for (int i = 0; i < DIRECTION_SIZE; ++i)
    PortRange[i] = NULL;

  map<unsigned int, Range> clpRangeMap = pInitialisor->GetClpToRangeMap();
  spdlog::warn("size of rangemap: {0}", clpRangeMap.size());
  for (auto &i:clpRangeMap) {
    spdlog::warn("{0} : ({1}, {2}),({3},{4})", i.first, i.second.GetMaxRangeValue().GetX(),
                 i.second.GetMaxRangeValue().GetY(), i.second.GetMinRangeValue().GetX(),
                 i.second.GetMinRangeValue().GetY());

  }
  map<unsigned int, Range>::iterator clpRangeMapIterator = clpRangeMap.begin();
  // Find the range information of each CLP.
  while (clpRangeMapIterator != clpRangeMap.end()) {
    int clpDirection = (int) fRouter->GetDirectionByLpRank(
        clpRangeMapIterator->first);
    Range newRange = clpRangeMapIterator->second;
    // If the CLP range is (INT_MAX), it indicates CLP has no range
    if (newRange != Range(Point(INT_MAX, INT_MAX), Point(INT_MAX, INT_MAX))) {
      // Insert new range
      if (PortRange[clpDirection]) {
        PortRange[clpDirection]->MinMaxofTwoRanges(newRange);
      } else {
        PortRange[clpDirection] = new Range(newRange);
      }
    }
    ++clpRangeMapIterator;
  }
  // Assign these information to the RangePeriods of RangeRouting Table at each port.
  for (int i = 0; i < DIRECTION_SIZE; ++i) {
    fRangeRoutingTable[i]->InsertRangePeriod(fStartTime,
                                             RangePeriod(fStartTime, PortRange[i]));
    if (PortRange[i]) delete PortRange[i];
  }
  // Clean up clp range map
  clpRangeMap.clear();

  // Verify the local port range information again for correctness.
  Range *newRange = fSharedState.RecalculateRange(fStartTime);
  Range *storedRange = fRangeRoutingTable[HERE]->GetRangeCopy(fStartTime);
  // If the recalculated range and the stored range aren't NULL and are not the same, update everything
  if ((newRange && storedRange) && (*newRange != *storedRange)) {
    fRangeRoutingTable[HERE]->DeleteRangePeriod(fStartTime);
    fRangeRoutingTable[HERE]->InsertRangePeriod(fStartTime,
                                                RangePeriod(fStartTime, newRange));
    list<RangeUpdates> rangeUpdateList;
    rangeUpdateList.push_back(RangeUpdates(fStartTime, newRange, storedRange));
    SendRangeUpdates(rangeUpdateList, HERE);
  }
  // Delete the memory allocated by ReCalculateRange function.
  if (newRange) delete newRange;
  if (storedRange) delete storedRange;
  fRangeRoutingTable[HERE]->ClearRangeUpdates();
}

#endif
