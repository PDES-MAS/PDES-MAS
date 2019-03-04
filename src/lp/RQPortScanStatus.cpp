#include "RQPortScanStatus.h"

using namespace std;
using namespace pdesmas;

RQPortScanStatus::RQPortScanStatus(Direction pOriginPort, unsigned int pNumberOfHops, const SerialisableMap<SsvId, Value<Point> >& pSSVValueMap) :
  fOriginPort(pOriginPort), fNumberOfHops(pNumberOfHops), fSSVValueMap(pSSVValueMap) {
  for (int port = 0; port < DIRECTION_SIZE; ++port) {
    if (port == pOriginPort) fPortScanStatusMap.insert( make_pair((Direction) port, AVOID_SCAN));
    else fPortScanStatusMap.insert(make_pair((Direction) port, NOT_SCANNED));
  }
}

RQPortScanStatus::~RQPortScanStatus() {
  fPortScanStatusMap.clear();
  fSSVValueMap.clear();
}

void RQPortScanStatus::SetScanStatus(Direction pPort, ScanStatus pStatus) {
  fPortScanStatusMap[pPort] = pStatus;
}

ScanStatus RQPortScanStatus::GetScanStatus(Direction pPort) const {
  map<Direction, ScanStatus>::const_iterator iter = fPortScanStatusMap.find(pPort);
  if (iter != fPortScanStatusMap.end()) return iter->second;
  return AVOID_SCAN;
}

void RQPortScanStatus::AddToSSVValueMap(const SerialisableMap<SsvId, Value<Point> >& pValueList) {
  SerialisableMap<SsvId, Value<Point> >::const_iterator valueIterator = pValueList.begin();
  while (valueIterator != pValueList.end()) {
    fSSVValueMap.insert(make_pair(valueIterator->first, valueIterator->second));
    ++valueIterator;
  }
}

const SerialisableMap<SsvId, Value<Point> >& RQPortScanStatus::GetSSVValueMap() const {
  return fSSVValueMap;
}

void RQPortScanStatus::SetOriginPort(Direction pPort) {
  fOriginPort = pPort;
}

Direction RQPortScanStatus::GetOriginPort() const {
  return fOriginPort;
}

int RQPortScanStatus::GetNumberofHops() const {
  return fNumberOfHops;
}

void RQPortScanStatus::IncreaseNumberOfHops(unsigned int pNumberOfHops) {
  fNumberOfHops += pNumberOfHops;
}
