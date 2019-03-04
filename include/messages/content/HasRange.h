/*
 * HasRange.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASRANGE_H_
#define HASRANGE_H_

#include "Range.h"

namespace pdesmas {
  class HasRange {
    protected:
      Range fRange;
    public:
      const Range& GetRange() const;
      void SetRange(const Range&);
  };
}

#endif /* HASRANGE_H_ */
