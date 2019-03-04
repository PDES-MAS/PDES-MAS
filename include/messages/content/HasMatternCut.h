/*
 * HasMatternCut.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASMATTERNCUT_H_
#define HASMATTERNCUT_H_

#include "MatternCut.h"
using std::ostream;
using std::istream;

namespace pdesmas {
  class HasMatternCut {
    protected:
      MatternCut fMatternCut;
    public:
      MatternCut GetMatternCut() const;
      void SetMatternCut(MatternCut);
  };
}

#endif /* HASMATTERNCUT_H_ */
