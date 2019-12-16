/*
 * IdentifierHandler.cpp
 *
 *  Created on: 4 Dec 2010
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include <limits.h>
#include "IdentifierHandler.h"

using namespace pdesmas;

IdentifierHandler::IdentifierHandler(unsigned int pRank, unsigned int pNumberOfClps, unsigned int pNumberOfAlps) :
    fInitialID(pRank - pNumberOfClps), fAdditional(pNumberOfAlps), fLastID(fInitialID) {
  lock_ = Mutex(NORMAL);
}

unsigned long IdentifierHandler::GetNextID() {
  lock_.Lock();
  // Handle tick over ULONG_MAX
  if (fLastID > (ULONG_MAX - fAdditional)) fLastID = fInitialID;
  else fLastID += fAdditional;
  lock_.Unlock();
  return fLastID;
}

unsigned long IdentifierHandler::GetLastID() const {
  return fLastID;
}
