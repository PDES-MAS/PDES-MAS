/*
 * HasGVT.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASGVT_H_
#define HASGVT_H_

namespace pdesmas {
  class HasGVT {
    protected:
      unsigned long fGVT;
    public:
      unsigned long GetGVT() const;
      void SetGVT(unsigned long);
  };
}

#endif /* HASGVT_H_ */
