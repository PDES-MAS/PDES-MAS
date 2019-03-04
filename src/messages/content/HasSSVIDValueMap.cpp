/*
 * HasSSVIDValueMap.cpp
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasSSVIDValueMap.h"

using namespace pdesmas;

const SerialisableMap<SsvId, Value<Point> >& HasSSVIDValueMap::GetSsvValueList() const {
  return fSsvIdValueMap;
}

void HasSSVIDValueMap::SetSsvValueList(const SerialisableMap<SsvId, Value<Point> >& pSsvIdValueMap) {
  fSsvIdValueMap = pSsvIdValueMap;
}

void HasSSVIDValueMap::SetValueForVariable(const SsvId& pSsvID, const Value<Point>& pValue) {
  fSsvIdValueMap.insert(make_pair(pSsvID, pValue));
}

void HasSSVIDValueMap::ClearSsvList() {
  fSsvIdValueMap.clear();
}
