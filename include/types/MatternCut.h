/*
 * MatternCut.h
 *
 *  Created on: 17 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef MATTERNCUT_H_
#define MATTERNCUT_H_

#include <iostream>
using std::ostream;
using std::istream;

namespace pdesmas {
  enum MatternCut {
    FIRST, SECOND
  };

  inline istream& operator>>(istream& pIstream, MatternCut& pMatternCut) {
    int matternCut;
    pIstream >> matternCut;
    if (matternCut == 0) pMatternCut = FIRST;
    if (matternCut == 1) pMatternCut = SECOND;
    return pIstream;
  }

  inline ostream& operator<<(ostream& pOstream, MatternCut const& pMatternCut) {
    if (pMatternCut == FIRST) pOstream << 0;
    if (pMatternCut == SECOND) pOstream << 1;
    return pOstream;
  }
}

#endif /* MATTERNCUT_H_ */
