#include <list>
#include "RangeTracker.h"

using namespace std;
using namespace pdesmas;

RangeTracker::RangeTracker(){
  // Empty
}

RangeTracker::~RangeTracker(){
  // Empty
}

bool RangeTracker::HasEntry(unsigned long pIdentifier) const {
  map<unsigned long, RQPortScanStatus>::const_iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
	if(rangeQueryPortIterator == fRangeQueryPortStatus.end())
		return false;
	return true;
}

void RangeTracker::CreateEntry(unsigned long pIdentifier, const SerialisableMap<SsvId, Value<Point> >& pSSVValueList, Direction pPort, int pNumberOfHops) {
  RQPortScanStatus rangeQueryPortStatus(pPort, pNumberOfHops, pSSVValueList);
  fRangeQueryPortStatus.insert(make_pair(pIdentifier, rangeQueryPortStatus));
}

Direction RangeTracker::DeleteEntry(unsigned long pIdentifier) {
  map<unsigned long, RQPortScanStatus>::iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
  if (rangeQueryPortIterator != fRangeQueryPortStatus.end()) {
    Direction originPort = (rangeQueryPortIterator->second).GetOriginPort();
    fRangeQueryPortStatus.erase(rangeQueryPortIterator);
    return originPort;
  }
  return HERE;
}

void RangeTracker::IncreaseNumberOfHops(unsigned long pIdentifier, unsigned int pNumberOfHops) {
  map<unsigned long, RQPortScanStatus>::iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
  if (rangeQueryPortIterator != fRangeQueryPortStatus.end()) {
    (rangeQueryPortIterator->second).IncreaseNumberOfHops(pNumberOfHops);
  }
}

int RangeTracker::GetNumberofHops(unsigned long pIdentifier) const {
  map<unsigned long, RQPortScanStatus>::const_iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
  if (rangeQueryPortIterator != fRangeQueryPortStatus.end()) {
    return ((rangeQueryPortIterator->second).GetNumberofHops());
  }
  return 0;
}

list<Direction> RangeTracker::GetOutstandingPorts(unsigned long pIdentifier) const {
  list<Direction> outstandingPortList;
  map<unsigned long, RQPortScanStatus>::const_iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
  if (rangeQueryPortIterator != fRangeQueryPortStatus.end()) {
    for (int port = 0; port < DIRECTION_SIZE; port++)
      if ((rangeQueryPortIterator->second).GetScanStatus((Direction) port) == NOT_SCANNED) {
        outstandingPortList.push_back((Direction) port);
      }
  }
  return outstandingPortList;
}

void RangeTracker::SetPortToScanned(unsigned long pIdentifier, Direction pPort) {
  map<unsigned long, RQPortScanStatus>::iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
  if (rangeQueryPortIterator != fRangeQueryPortStatus.end()) {
    (rangeQueryPortIterator->second).SetScanStatus(pPort, SCANNED);
  }
}

void RangeTracker::AddToSSVValueMap(unsigned long pIdentifier, const SerialisableMap<SsvId, Value<Point> >& pSSVValueList) {
  map<unsigned long, RQPortScanStatus>::iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
  if (rangeQueryPortIterator != fRangeQueryPortStatus.end()) {
    (rangeQueryPortIterator->second).AddToSSVValueMap(pSSVValueList);
  }
}

SerialisableMap<SsvId, Value<Point> > RangeTracker::GetCollectedSSVValueMap( unsigned long pIdentifier) {
  SerialisableMap<SsvId, Value<Point> > valueList;
  map<unsigned long, RQPortScanStatus>::iterator rangeQueryPortIterator = fRangeQueryPortStatus.find(pIdentifier);
  if (rangeQueryPortIterator != fRangeQueryPortStatus.end()) {
    valueList = (rangeQueryPortIterator->second).GetSSVValueMap();
    return valueList;
  }
  return valueList;
}

