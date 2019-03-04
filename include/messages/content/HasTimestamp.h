/*
 * HasTimestamp.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASTIMESTAMP_H_
#define HASTIMESTAMP_H_

namespace pdesmas {
  class HasTimestamp {
    protected:
      unsigned long fTimestamp;
    public:
      unsigned long GetTimestamp() const;
      void SetTimestamp(unsigned long);
  };
}

#endif /* HASTIMESTAMP_H_ */
