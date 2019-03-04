/*
 * HasMatternColour.h
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASMATTERNCOLOUR_H_
#define HASMATTERNCOLOUR_H_

#include "MatternColour.h"

namespace pdesmas {
  class HasMatternColour {
    protected:
      MatternColour fMatternColour;
    public:
      MatternColour GetMatternColour() const;
      void SetMatternColour(MatternColour pMatternColour);
  };
}

#endif /* HASMATTERNCOLOUR_H_ */
