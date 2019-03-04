/*
 * HasRollbackTagList.cpp
 *
 *  Created on: 12 Apr 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include <algorithm>
#include "HasRollbackTagList.h"

using namespace pdesmas;

bool HasRollbackTagList::CheckRollbackTagList(const RollbackTag pRollbackTag) {
  list<RollbackTag>::iterator iter = find(fRollbackTagList.begin(), fRollbackTagList.end(), pRollbackTag);
  if (iter != fRollbackTagList.end()) {
    return true;
  }
  fRollbackTagList.push_back(pRollbackTag);
  return false;
}

void HasRollbackTagList::ClearRollbackTagList(unsigned long pTime) {
  for (list<RollbackTag>::iterator iter = fRollbackTagList.begin(); iter
      != fRollbackTagList.end();) {
    if (iter->GetTime() < pTime) {
      iter = fRollbackTagList.erase(iter);
    } else iter++;
  }
}
