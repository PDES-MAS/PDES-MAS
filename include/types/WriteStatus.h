/*
 * WriteStatus.h
 *
 *  Created on: 17 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef WRITESTATUS_H_
#define WRITESTATUS_H_

#include <iostream>
using std::istream;
using std::ostream;

namespace pdesmas {
  enum WriteStatus {
    writeSUCCESS, writeFAILURE
  };

  inline istream& operator>>(istream& pIstream, WriteStatus& pWriteStatus) {
    int writeStatus;
    pIstream >> writeStatus;
    if (writeStatus == 0) pWriteStatus = writeSUCCESS;
    if (writeStatus == 1) pWriteStatus = writeFAILURE;
    return pIstream;
  }

  inline ostream& operator<<(ostream& pOstream, WriteStatus const& pWriteStatus) {
    if (pWriteStatus == writeSUCCESS) pOstream << 0;
    if (pWriteStatus == writeFAILURE) pOstream << 1;
    return pOstream;
  }
}

#endif /* WRITESTATUS_H_ */
