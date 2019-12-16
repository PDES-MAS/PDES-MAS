/*
 * IdentifierHandler.h
 *
 *  Created on: 4 Dec 2010
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef IDENTIFIERHANDLER_H_
#define IDENTIFIERHANDLER_H_

#include <util/Mutex.h>
#include "HasMPIInfo.h"

namespace pdesmas {
  class IdentifierHandler {
    private:
      const unsigned long fInitialID;
      const unsigned long fAdditional;
      unsigned long fLastID;
      Mutex lock_;
    public:
      IdentifierHandler(unsigned int, unsigned int, unsigned int);

      unsigned long GetNextID();
      unsigned long GetLastID() const;
  };
}

#endif /* IDENTIFIERHANDLER_H_ */
