#include "StateVariable.h"
#include <climits>
#include "spdlog/spdlog.h"

using namespace pdesmas;

StateVariable::StateVariable() {
  // Empty
}

StateVariable::StateVariable(const SsvId &pSSVID) {
  fStateVariableID = pSSVID;
}

StateVariable::StateVariable(const StateVariable &pStateVariable) {
  // Copy the identifier
  fStateVariableID = SsvId(pStateVariable.fStateVariableID);
  // Copy the write period list, assignment also copies elements
  fWritePeriodList = pStateVariable.fWritePeriodList;
}

StateVariable::~StateVariable() {
  // Empty deconstructor
}

const SsvId &StateVariable::GetVariableId() const {
  return fStateVariableID;
}

const SerialisableList <WritePeriod> &StateVariable::GetWritePeriodList() const {
  return fWritePeriodList;
}

void StateVariable::AddWritePeriod(const AbstractValue *pValue, unsigned long pTime, const LpId &pAgentID) {
  fWritePeriodList.push_back(WritePeriod(pValue, pTime, pAgentID));
}

void StateVariable::RemoveWritePeriods(unsigned long pTime) {
  LOG(logFINEST) << "StateVariable::RemoveWritePeriods# Remove write periods up to: " << pTime;
  SerialisableList<WritePeriod>::reverse_iterator reverseWritePeriodIterator = fWritePeriodList.rbegin();
  // Walk backwards to the list until the reach the write period with an equal or less than start time then the parameter time
  while (reverseWritePeriodIterator != fWritePeriodList.rend()) {
    if (reverseWritePeriodIterator->GetStartTime() <= pTime) break;
    ++reverseWritePeriodIterator; //Not this one so check next in list
  }
  // If we've found the write period in the list
  if ((reverseWritePeriodIterator != fWritePeriodList.rend())) {
    LOG(logFINEST) << "StateVariable::RemoveWritePeriods# Found write period: " << *reverseWritePeriodIterator;
    // Set the new start time
    reverseWritePeriodIterator->SetStartTime(pTime);
    LOG(logFINEST) << "StateVariable::RemoveWritePeriods# Reset start time to: " << pTime << ", writeperiod: "
                   << *reverseWritePeriodIterator;
    // Remove all reads from before the time
    reverseWritePeriodIterator->RemoveReadsBefore(pTime);
    // Remove all write periods up to the downward write period iterator
    SerialisableList<WritePeriod>::iterator writePeriodIterator = fWritePeriodList.begin();
    while (writePeriodIterator->GetStartTime() != reverseWritePeriodIterator->GetStartTime()) {
      LOG(logFINEST) << "StateVariable::RemoveWritePeriods# Erase write period: " << *writePeriodIterator;
      writePeriodIterator = fWritePeriodList.erase(writePeriodIterator);
      if (writePeriodIterator == fWritePeriodList.end()) break;
    }
  }
  LOG(logFINEST) << "StateVariable::RemoveWritePeriods# Remaining write period list: ";
  for (SerialisableList<WritePeriod>::iterator writePeriodIterator =
      fWritePeriodList.begin(); writePeriodIterator != fWritePeriodList.end(); ++writePeriodIterator) {
    LOG(logFINEST)
        << "StateVariable::RemoveWritePeriod# " << *writePeriodIterator;
  }
  LOG(logFINEST) << "StateVariable::RemoveWritePeriods# End of write period list.";
}

AbstractValue *StateVariable::Read(const LpId &pReadingAgent, unsigned long pTime) {
  SerialisableList<WritePeriod>::reverse_iterator
      reverseWritePeriodIterator = fWritePeriodList.rbegin();
  while (reverseWritePeriodIterator != fWritePeriodList.rend()) {
    if (reverseWritePeriodIterator->GetStartTime() <= pTime) break;
    ++reverseWritePeriodIterator; //Not this one so check next in list
  }
  if (reverseWritePeriodIterator == fWritePeriodList.rend()) {
    spdlog::critical(
        "StateVariable::SendReadMessageAndGetResponse: Could not find a write period, id: {}, reading agent: {}, time: {}",
        fStateVariableID.id(), pReadingAgent.GetId(), pTime);

    for (SerialisableList<WritePeriod>::iterator writePeriodIterator =
        fWritePeriodList.begin(); writePeriodIterator != fWritePeriodList.end(); ++writePeriodIterator) {
      ostringstream out;
      writePeriodIterator->Serialise(out);
      spdlog::critical("StateVariable::SendReadMessageAndGetResponse {}", out.str());
    }
    exit(1);
  }
  //SendReadMessageAndGetResponse the write period
  return reverseWritePeriodIterator->Read(pReadingAgent, pTime);
}

pair<unsigned long, AbstractValue *> StateVariable::ReadWithoutRecord(unsigned long pTime) const {
  SerialisableList<WritePeriod>::const_reverse_iterator reverseWritePeriodIterator = fWritePeriodList.rbegin();
  while (reverseWritePeriodIterator != fWritePeriodList.rend()) {
    if (reverseWritePeriodIterator->GetStartTime() <= pTime) break;
    ++reverseWritePeriodIterator; //Not this one so check next in list
  }
  //Add to readlist of write period
  if (reverseWritePeriodIterator != fWritePeriodList.rend()) {
    return make_pair(reverseWritePeriodIterator->GetEndTime(), reverseWritePeriodIterator->GetValueCopy());
  }
  AbstractValue *value = NULL;
  return make_pair(ULONG_MAX, value);
}

void StateVariable::WriteWithRollback(const LpId &pWritingAgent, const AbstractValue *pValue, unsigned long pTime,
                                      WriteStatus &pWriteStatus, RollbackList &pRollbackList) {
  // Create the new write period
  WritePeriod newWritePeriod(pValue, pTime, pWritingAgent);
  // If the list is empty, just pushback the new write period
  if (fWritePeriodList.empty()) {
    fWritePeriodList.push_back(newWritePeriod);
    pWriteStatus = writeSUCCESS;
    return;
  }
  // The list is not empty, so reverse walk to write period just before new write period in time
  auto reverseWritePeriodIterator = fWritePeriodList.rbegin();
  while (reverseWritePeriodIterator != fWritePeriodList.rend()) {
    if (reverseWritePeriodIterator->GetStartTime() <= pTime) break;
    ++reverseWritePeriodIterator; //Not this one so check next in list
  }
  // Walked through the entire list and didn't find a 'before' write period!
  if (reverseWritePeriodIterator == fWritePeriodList.rend()) {
    LOG(logWARNING)
        << "StateVariable::WriteWithRollback# Didn't find before write period, writing agent: "
        << pWritingAgent << ", value: " << pValue << ",                           time: " << pTime
        << ", write status: " << pWriteStatus;
    pWriteStatus = writeFAILURE;
    return;
  }
  // Reject any write at the same time from different agents (using a tie-breaker)
  if (reverseWritePeriodIterator->GetStartTime() == pTime
      && reverseWritePeriodIterator->GetAgent() > pWritingAgent) {
    spdlog::warn("StateVariable::WriteWithRollback# Write failed! (writing at same time)");
    pWriteStatus = writeFAILURE;
    return;
  }
  // Set end time for write period in the list
  reverseWritePeriodIterator->SetEndTime(newWritePeriod.GetStartTime());
  // Get rollback list for all invalidated reads after time
  reverseWritePeriodIterator->RemoveReadsAfterInclusive(pTime, pRollbackList);
  // If there is a write period ahead (we split a write period), set end time for new write period
  if (reverseWritePeriodIterator != fWritePeriodList.rbegin()) {
    // Move iterator one to the right
    --reverseWritePeriodIterator;
    newWritePeriod.SetEndTime(reverseWritePeriodIterator->GetStartTime());
    // Move iterator one to the left
    ++reverseWritePeriodIterator;
    // Insert write period just before the just moved iterator (base reverse iterator point to one to the right)
    fWritePeriodList.insert(reverseWritePeriodIterator.base(), newWritePeriod);
    pWriteStatus = writeSUCCESS;
    return;
  }
  // We have not split a write period, so we can append the new write period to the list
  fWritePeriodList.push_back(newWritePeriod);
  pWriteStatus = writeSUCCESS;
}

void StateVariable::PerformReadRollback(const LpId &pWritingAgent, unsigned long pTime) {
  SerialisableList<WritePeriod>::reverse_iterator reverseWritePeriodIterator = fWritePeriodList.rbegin();
  while (reverseWritePeriodIterator != fWritePeriodList.rend()) {
    if (reverseWritePeriodIterator->GetStartTime() <= pTime) break;
    ++reverseWritePeriodIterator; //Not this one so check next in list
  }
  reverseWritePeriodIterator->RemoveReadsByAgent(pWritingAgent, pTime);
}

void StateVariable::PerformWriteRollback(const LpId &pWritingAgent, unsigned long pTime, RollbackList &pRollbackList) {
  // Walk through write period list to look for write period to roll back
  SerialisableList<WritePeriod>::iterator writePeriodIterator =
      fWritePeriodList.begin();
  while (writePeriodIterator != fWritePeriodList.end()) {
    if (writePeriodIterator->GetStartTime() == pTime) break;
    ++writePeriodIterator;
  }
  // If write period is not found print warning and return
  if (writePeriodIterator == fWritePeriodList.end()) {
    LOG(logWARNING)
        << "StateVariable::PerformWriteRollback# Can't find Write Period for Rollback: "
        << pWritingAgent << ", at time: " << pTime;
    LOG(logWARNING)
        << "StateVariable::PerformWriteRollback# Write period list:";
    for (typename SerialisableList<WritePeriod>::iterator i =
        fWritePeriodList.begin(); i != fWritePeriodList.end(); ++i) {
      LOG(logWARNING)
          << "StateVariable::PerformWriteRollback# " << *i;
    }
    LOG(logWARNING)
        << "StateVariable::PerformWriteRollback# End of write period list.";
    return;
  }
  // If write period agent is not writing agent, print warning and return
  if (writePeriodIterator->GetAgent() != pWritingAgent) {
    LOG(logWARNING)
        << "StateVariable::PerformWriteRollback# Rollback attempted by non-owner, writing agent: "
        << pWritingAgent << ", owner: " << writePeriodIterator->GetAgent()
        << ", time: " << pTime;
    return;
  }
  // Add all reads to rollback list
  writePeriodIterator->RemoveAllReads(pTime, pRollbackList);
  // If element is the only one in the list
  if (fWritePeriodList.size() == 1) {
    LOG(logFINEST)
        << "StateVariable::PerformWriteRollback# Rollback attempted on single write period in list, writing agent: "
        << pWritingAgent << ", owner: " << writePeriodIterator->GetAgent()
        << ", time: " << pTime;
    // Erase the existing write period
    fWritePeriodList.erase(writePeriodIterator);
    // Return rollback list
    return;
  }
  // If element is last in the list
  if (writePeriodIterator == (++fWritePeriodList.rbegin()).base()) {
    // Move iterator to previous one
    --writePeriodIterator;
    // Set previous write period end time to infinite
    writePeriodIterator->SetEndTime(LONG_MAX);
    // Move iterator back to found write period
    ++writePeriodIterator;
    // Erase the existing write period
    fWritePeriodList.erase(writePeriodIterator);
    // Return rollback list
    return;
  }
  // If element is first in the list
  if (writePeriodIterator == fWritePeriodList.begin()) {
    LOG(logFINEST)
        << "StateVariable::PerformWriteRollback# Rollback attempted on first in write period list, writing agent: "
        << pWritingAgent << ", owner: " << writePeriodIterator->GetAgent()
        << ", time: " << pTime;
    // Store start time
    //unsigned long erasedStartTime = writePeriodIterator->GetStartTime();
    // Move iterator one ahead
    //++writePeriodIterator;
    // Set start time of next write period to erased start time
    //writePeriodIterator->SetStartTime(erasedStartTime);
    // Move iterator back
    //--writePeriodIterator;
    // Erase the existing write period
    fWritePeriodList.erase(writePeriodIterator);
    // Return rollback list
    return;
  }
  // Element is in the middle of the list
  LOG(logFINEST)
      << "StateVariable::PerformWriteRollback# Rollback attempted on middle in write period list, writing agent: "
      << pWritingAgent << ", owner: " << writePeriodIterator->GetAgent()
      << ", time: " << pTime;
  // Store end time
  unsigned long erasedEndTime = writePeriodIterator->GetEndTime();
  // Move iterator one back
  --writePeriodIterator;
  // Set end time of previous write period to erased end time
  writePeriodIterator->SetEndTime(erasedEndTime);
  // Move iterator back to original position
  ++writePeriodIterator;
  // Erase the existing write period
  fWritePeriodList.erase(writePeriodIterator);
  // Return rollback list
  return;
}

void StateVariable::Serialise(ostream &pOstream) const {
  pOstream << DELIM_LEFT << fStateVariableID;
  pOstream << DELIM_VAR_SEPARATOR << fWritePeriodList;
  pOstream << DELIM_RIGHT;
}

void StateVariable::Deserialise(istream &pIstream) {
  IgnoreTo(pIstream, DELIM_LEFT);
  pIstream >> fStateVariableID;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fWritePeriodList;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
