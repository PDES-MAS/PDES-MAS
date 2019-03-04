#include "WritePeriod.h"
#include <climits>

using namespace pdesmas;

WritePeriod::WritePeriod() {
  fStartTime = 0;
  fEndTime = ULONG_MAX;
  fValue = NULL;
  fAgent = LpId(0,0);
}

WritePeriod::WritePeriod(const AbstractValue* pValue, unsigned long pStartTime, const LpId& pAgent) {
  fStartTime = pStartTime;
  fEndTime = ULONG_MAX;
  // Copy the value!
  SetValue(pValue);
  fAgent = pAgent;
}

WritePeriod::WritePeriod(const WritePeriod& pWritePeriod) {
  fStartTime = pWritePeriod.fStartTime;
  fEndTime = pWritePeriod.fEndTime;
  // Copy the value!
  SetValue(pWritePeriod.fValue);
  // Copy the agent!
  fAgent = LpId(pWritePeriod.fAgent);
  // Deep copy the agent read map, assignment copies elements
  fAgentReadMap = pWritePeriod.fAgentReadMap;
}

WritePeriod::~WritePeriod() {
  if (fValue != NULL) {
    delete fValue;
    fValue = NULL;
  }
}

void WritePeriod::SetStartTime(unsigned long pTime) {
  fStartTime = pTime;
}

unsigned long WritePeriod::GetStartTime() const {
  return fStartTime;
}

void WritePeriod::SetAgent(const LpId& pAgent) {
  fAgent = pAgent;
}

const LpId& WritePeriod::GetAgent() const {
  return fAgent;
}

void WritePeriod::SetEndTime(unsigned long pTime) {
  fEndTime = pTime;
}

unsigned long WritePeriod::GetEndTime() const {
  return fEndTime;
}

void WritePeriod::SetValue(const AbstractValue* pValue) {
  if (pValue == NULL) {
    return;
  }
  fValue = pValue->Clone();
}

AbstractValue* WritePeriod::GetValueCopy() const {
  if (fValue == NULL)
    return NULL;
  return fValue->Clone();
}

AbstractValue* WritePeriod::Read(const LpId& pAgent, unsigned long pTime) {
  fAgentReadMap.insert(make_pair(pAgent, pTime));
  return GetValueCopy();
}

void WritePeriod::RemoveReadsBefore(unsigned long pTime) {
  SerialisableMultiMap<LpId, unsigned long>::iterator agentReadMapIterator = fAgentReadMap.begin();
  while (agentReadMapIterator != fAgentReadMap.end()) {
    if (agentReadMapIterator->second < pTime) fAgentReadMap.erase(agentReadMapIterator++);
    else ++agentReadMapIterator;
  }
}

void WritePeriod::RemoveReadsAfterInclusive(unsigned long pTime, RollbackList& pRollbackList) {
  SerialisableMultiMap<LpId, unsigned long>::iterator agentReadMapIterator = fAgentReadMap.begin();
  while (agentReadMapIterator != fAgentReadMap.end()) {
    if (agentReadMapIterator->second >= pTime) {
      pRollbackList.AddLp(agentReadMapIterator->first, agentReadMapIterator->second);
      fAgentReadMap.erase(agentReadMapIterator++);
    } else ++agentReadMapIterator;
  }
}

void WritePeriod::RemoveReadsByAgent(const LpId& pAgent, unsigned long pTime) {
  SerialisableMultiMap<LpId, unsigned long>::iterator agentReadMapIterator = fAgentReadMap.begin();
  while (agentReadMapIterator != fAgentReadMap.end()) {
    LpId agent = agentReadMapIterator->first;
    if (agent == pAgent && agentReadMapIterator->second == pTime) fAgentReadMap.erase(agentReadMapIterator++);
    else ++agentReadMapIterator;
  }
}

void WritePeriod::RemoveAllReads(unsigned long pTime, RollbackList& pRollbackList) {
  SerialisableMultiMap<LpId, unsigned long>::iterator agentReadMapIterator = fAgentReadMap.begin();
  while (agentReadMapIterator != fAgentReadMap.end()) {
    pRollbackList.AddLp(agentReadMapIterator->first, pTime);
    fAgentReadMap.erase(agentReadMapIterator++);
  }
}

void WritePeriod::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << fStartTime;
  pOstream << DELIM_VAR_SEPARATOR << fEndTime;
  pOstream << DELIM_VAR_SEPARATOR << fAgent;
  pOstream << DELIM_VAR_SEPARATOR << fAgentReadMap;
  pOstream << DELIM_VAR_SEPARATOR << *fValue;
  pOstream << DELIM_RIGHT;
}

void WritePeriod::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_LEFT);
  pIstream >> fStartTime;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fEndTime;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fAgent;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fAgentReadMap;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  IgnoreTo(pIstream, DELIM_LEFT);
  string valueString;
  getline(pIstream, valueString, DELIM_LIST_RIGHT);
  string value = GetValueString(valueString);
  fValue = valueClassMap->CreateObject(GetTypeID(valueString));
  fValue->SetValue(value);
  pIstream.unget();
}
