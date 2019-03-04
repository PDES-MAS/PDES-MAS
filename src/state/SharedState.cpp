# include "SharedState.h"

using namespace pdesmas;

SharedState::SharedState() {
  fRangeRoutingTable = NULL;
  fAccessCostCalculator = NULL;
}

SharedState::~SharedState() {
  // Empty
}

void SharedState::SetRangeRoutingTable(RangeRoutingTable* pRangeRoutingTable) {
  fRangeRoutingTable = pRangeRoutingTable;
}

void SharedState::SetAccessCostCalculator(AccessCostCalculator* pAccessCostCalculator) {
  fAccessCostCalculator = pAccessCostCalculator;
}

void SharedState::UpdateAccessCount(const SsvId& pSSVID, Direction pDirection, unsigned long pNumberOfHops) {
  unsigned long access, hops;
  access = fAccessCostCalculator->UpdateAccessCount(pDirection, 1, pSSVID);
  hops = fAccessCostCalculator->UpdateHopCount(pDirection, pNumberOfHops, pSSVID);
  fAccessCostCalculator->UpdateLoad(hops, access, pNumberOfHops + hops, access + 1);
}

bool SharedState::ContainsVariable(const SsvId& pSSVID) {
  return (fStateVariableMap.count(pSSVID) > 0);
}

void SharedState::Add(const SsvId& pSSVID, const AbstractValue* pValue, unsigned long pTime, const LpId& pAgentID) {
  if (ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::Add(SsvID,AbstractValue*,unsigned long,LpId)# trying to add a variable that already exists";
    exit(1);
  }
  StateVariable newStateVariable(pSSVID);
  newStateVariable.AddWritePeriod(pValue, pTime, pAgentID);
  fStateVariableMap[pSSVID] = newStateVariable;

  #ifdef SSV_LOCALISATION
  fAccessCostCalculator->InitialiseCounters(pSSVID);
#endif
}

void SharedState::Insert(const SsvId& pSSVID, const StateVariable& pStateVariable, RollbackList& pRollbackList) {
  if (ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::Insert# Trying to insert a shared state variable that already exists!";
    exit(1);
  }
  fStateVariableMap[pSSVID] = StateVariable(pSSVID);
  map<SsvId, StateVariable>::iterator stateVariableMapIterator = fStateVariableMap.find(pSSVID);
  SerialisableList<WritePeriod> writePeriodList = pStateVariable.GetWritePeriodList();
  list<WritePeriod>::iterator writePeriodListIterator = writePeriodList.begin();

  while (writePeriodListIterator != writePeriodList.end()) {
#ifdef RANGE_QUERIES
    pair<unsigned long, AbstractValue*> timeValuePair = stateVariableMapIterator->second.ReadWithoutRecord(writePeriodListIterator->GetStartTime());
    Point* oldValue = NULL;
    if (timeValuePair.second) {
      if (VALUEPOINT == timeValuePair.second->GetType()) {
        oldValue = new Point(static_cast<Value<Point>* >(timeValuePair.second)->GetValue());
      }
      delete timeValuePair.second;
    }
#endif

    AbstractValue* value = writePeriodListIterator->GetValueCopy();
    WriteStatus status;
    stateVariableMapIterator->second.WriteWithRollback(writePeriodListIterator->GetAgent(), value, writePeriodListIterator->GetStartTime(), status, pRollbackList);

#ifdef RANGE_QUERIES
    Point* newValue = NULL;
    if (value) {
      if (VALUEPOINT == value->GetType()) {
        newValue = new Point(static_cast<const Value<Point>* >(value)->GetValue());
      }
    }
    if (oldValue || newValue) {
      Range* newRange = RecalculateRange(writePeriodListIterator->GetStartTime());
      fRangeRoutingTable->Update(oldValue, newValue, writePeriodListIterator->GetStartTime(), newRange, this, pRollbackList, writePeriodListIterator->GetEndTime());
      if (newRange) delete newRange;
    }
    if (oldValue) delete oldValue;
    if (newValue) delete newValue;
#endif
    if (value) delete value;
    ++writePeriodListIterator;
  }

#ifdef SSV_LOCALISATION
  fAccessCostCalculator->InitialiseCounters(pSSVID);
#endif
}

void SharedState::Delete(const SsvId& pSSVID) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::Delete# trying to delete a variable that does not exists";
    exit(1);
  }
  fStateVariableMap.erase(pSSVID);
#ifdef SSV_LOCALISATION
  fAccessCostCalculator->RemoveSsvAccessRecord(pSSVID);
  fAccessCostCalculator->RemoveSsvHopRecord(pSSVID);
  fAccessCostCalculator->RemoveSsvFromList(pSSVID);
#endif
}

StateVariable SharedState::GetCopy(const SsvId& pSSVID) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::Get# Trying to get a non-existant variable!";
    exit(1);
  }
  return StateVariable(fStateVariableMap.find(pSSVID)->second);
}

AbstractValue* SharedState::Read(const SsvId& pSSVID, const LpId& pAgentID, unsigned long pTime) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::Read# trying to perform a read on statevariable that doesn't exist";
    exit(1);
  }
  return fStateVariableMap.find(pSSVID)->second.Read(pAgentID, pTime);
}

void SharedState::WriteWithRollback(const SsvId& pSSVID, const LpId& pAgentID, const AbstractValue* pNewValue, unsigned long pTime, WriteStatus& pWriteStatus, RollbackList& pRollbackList) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::WriteWithRollback# Trying to perform a write on statevariable that doesn't exist";
    exit(1);
  }
  map<SsvId, StateVariable>::iterator stateVariableMapIterator = fStateVariableMap.find(pSSVID);
#ifdef RANGE_QUERIES
    pair<unsigned long, AbstractValue*> timeValuePair = stateVariableMapIterator->second.ReadWithoutRecord(pTime);
    unsigned long endTime = timeValuePair.first;
    Point* oldValue = NULL;
    if (timeValuePair.second) {
      if (VALUEPOINT == timeValuePair.second->GetType()) {
        oldValue = new Point(static_cast<Value<Point>* >(timeValuePair.second)->GetValue());
      }
      delete timeValuePair.second;
    }
#endif

    stateVariableMapIterator->second.WriteWithRollback(pAgentID, pNewValue, pTime, pWriteStatus, pRollbackList);

#ifdef RANGE_QUERIES
    Point* newValue = NULL;
    if (pNewValue) {
      if (VALUEPOINT == pNewValue->GetType()) {
        newValue = new Point(static_cast<const Value<Point>* >(pNewValue)->GetValue());
      }
    }
    if (oldValue || newValue) {
      Range* newRange = RecalculateRange(pTime);
      fRangeRoutingTable->Update(oldValue, newValue, pTime, newRange, this, pRollbackList, endTime);
      if (newRange) delete newRange;
    }
    if (oldValue) delete oldValue;
    if (newValue) delete newValue;
#endif
}

SerialisableMap<SsvId, Value<Point> > SharedState::RangeRead(const Range& pRange, Direction pDirection, unsigned long pHops, unsigned long pTime) {
  SerialisableMap<SsvId, Value<Point> > pointMap;
  map<SsvId, StateVariable>::iterator stateVariableIterator = fStateVariableMap.begin();
  while (stateVariableIterator != fStateVariableMap.end()) {
    pair<unsigned long, AbstractValue*> timeValuePair = stateVariableIterator->second.ReadWithoutRecord(pTime);
    if (timeValuePair.second) {
      if (VALUEPOINT == timeValuePair.second->GetType()) {
        Value<Point>* pointValue = static_cast<Value<Point>* >(timeValuePair.second);
        if (pRange.IsValueOverlapping(pointValue->GetValue())) {
          pointMap.insert(make_pair(stateVariableIterator->first, *pointValue));
          UpdateAccessCount(stateVariableIterator->first, pDirection, pHops);
        }
      }
      delete timeValuePair.second;
    }
    ++stateVariableIterator;
  }
  return pointMap;
}

Range* SharedState::RecalculateRange(unsigned long pTime) const {
  map<SsvId, StateVariable>::const_iterator stateVariableIterator = fStateVariableMap.begin();
  Point* minimumPoint = NULL;
  Point* maximumPoint = NULL;
  while (stateVariableIterator != fStateVariableMap.end()) {
    pair<unsigned long, AbstractValue*> timeValuePair = stateVariableIterator->second.ReadWithoutRecord(pTime);
    if (timeValuePair.second) {
      if (VALUEPOINT == timeValuePair.second->GetType()) {
        Value<Point>* pointValue = static_cast<Value<Point>* >(timeValuePair.second);
        if (minimumPoint == NULL) minimumPoint = new Point(pointValue->GetValue());
        else minimumPoint->Min(pointValue->GetValue());
        if (maximumPoint == NULL) maximumPoint = new Point(pointValue->GetValue());
        else maximumPoint->Max(pointValue->GetValue());
      }
      delete timeValuePair.second;
    }
    ++stateVariableIterator;
  }
  if (maximumPoint && minimumPoint) {
    Range* newRange = new Range(*minimumPoint, *maximumPoint);
    delete minimumPoint;
    delete maximumPoint;
    return newRange;
  }
  return NULL;
}

void SharedState::RollbackWrite(const SsvId& pSSVID, const LpId& pAgentID, unsigned long pTime, RollbackList& pRollbackList) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::RollbackWrite# trying to perform a rollback on statevariable that doesn't exist";
    exit(1);
  }
  map<SsvId, StateVariable>::iterator stateVariableMapIterator = fStateVariableMap.find(pSSVID);
#ifdef RANGE_QUERIES
  pair<unsigned long, AbstractValue*> timeValuePair = stateVariableMapIterator->second.ReadWithoutRecord(pTime);
  unsigned long endTime = timeValuePair.first;
  Point* oldValue = NULL;
  if (timeValuePair.second) {
    if (VALUEPOINT == timeValuePair.second->GetType()) {
      oldValue = new Point(static_cast<Value<Point>* >(timeValuePair.second)->GetValue());
    }
    delete timeValuePair.second;
  }
#endif

  stateVariableMapIterator->second.PerformWriteRollback(pAgentID, pTime, pRollbackList);

#ifdef RANGE_QUERIES
  timeValuePair = stateVariableMapIterator->second.ReadWithoutRecord(pTime);
  Point* newValue = NULL;
  if (timeValuePair.second) {
    if (VALUEPOINT == timeValuePair.second->GetType()) {
      newValue = new Point(static_cast<const Value<Point>* >(timeValuePair.second)->GetValue());
    }
    delete timeValuePair.second;
  }
  if (oldValue || newValue) {
    Range* newRange = RecalculateRange(pTime);
    fRangeRoutingTable->Update(oldValue, newValue, pTime, newRange, this, pRollbackList, endTime);
    if (newRange) delete newRange;
  }
  if (oldValue) delete oldValue;
  if (newValue) delete newValue;
#endif
}

void SharedState::RollbackRead(const SsvId& pSSVID, const LpId& pAgentID, unsigned long pTime) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::RollbackRead# trying to perform a rollback on statevariable that doesn't exist";
    exit(1);
  }
  fStateVariableMap.find(pSSVID)->second.PerformReadRollback(pAgentID, pTime);
}

void SharedState::RemoveWritePeriods(unsigned long pTime) {
  map<SsvId, StateVariable>::iterator stateVariableMapIterator = fStateVariableMap.begin();
  while (stateVariableMapIterator != fStateVariableMap.end()) {
    stateVariableMapIterator->second.RemoveWritePeriods(pTime);
    ++stateVariableMapIterator;
  }
}

void SharedState::RemoveWritePeriodList(const SsvId& pSSVID, RollbackList& pRollbackList) {
  if (!ContainsVariable(pSSVID)) {
    LOG(logERROR) << "SharedState::RemoveWritePeriodList# Trying to remove write period list for non-existing SSV";
    exit(1);
  }
  map<SsvId, StateVariable>::iterator stateVariableMapIterator = fStateVariableMap.find(pSSVID);
  SerialisableList<WritePeriod> writePeriodList = stateVariableMapIterator->second.GetWritePeriodList();
  list<WritePeriod>::iterator writePeriodListIterator = writePeriodList.begin();
  while (writePeriodListIterator != writePeriodList.end()) {
#ifdef RANGE_QUERIES
    pair<unsigned long, AbstractValue*> timeValuePair = stateVariableMapIterator->second.ReadWithoutRecord(writePeriodListIterator->GetStartTime());
    Point* oldValue = NULL;
    if (timeValuePair.second) {
      if (VALUEPOINT == timeValuePair.second->GetType()) {
        oldValue = new Point(static_cast<Value<Point>* >(timeValuePair.second)->GetValue());
      }
      delete timeValuePair.second;
    }
#endif

    stateVariableMapIterator->second.PerformWriteRollback(writePeriodListIterator->GetAgent(), writePeriodListIterator->GetStartTime(), pRollbackList);

#ifdef RANGE_QUERIES
    timeValuePair = stateVariableMapIterator->second.ReadWithoutRecord(writePeriodListIterator->GetStartTime());
    Point* newValue = NULL;
    if (timeValuePair.second) {
      if (VALUEPOINT == timeValuePair.second->GetType()) {
        newValue = new Point(static_cast<const Value<Point>* >(timeValuePair.second)->GetValue());
      }
      delete timeValuePair.second;
    }
    if (oldValue || newValue) {
      Range* newRange = RecalculateRange(writePeriodListIterator->GetStartTime());
      fRangeRoutingTable->Update(oldValue, newValue, writePeriodListIterator->GetStartTime(), newRange, this, pRollbackList, writePeriodListIterator->GetEndTime());
      if (newRange) delete newRange;
    }
    if (oldValue) delete oldValue;
    if (newValue) delete newValue;
#endif

    ++writePeriodListIterator;
  }
}
