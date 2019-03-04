/*
 * MatternColour.h
 *
 *  Created on: 17 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef MATTERNCOLOUR_H_
#define MATTERNCOLOUR_H_

#include <iostream>
using std::istream;
using std::ostream;

namespace pdesmas {
  enum MatternColour {
    WHITE, RED
  };

  inline istream& operator>>(istream& pIstream, MatternColour& pMatternColour) {
    int matternColour;
    pIstream >> matternColour;
    if (matternColour == 0) pMatternColour = RED;
    if (matternColour == 1) pMatternColour = WHITE;
    return pIstream;
  }

  inline ostream& operator<<(ostream& pOstream, MatternColour const& pMatternColour) {
    if (pMatternColour == RED) pOstream << 0;
    if (pMatternColour == WHITE) pOstream << 1;
    return pOstream;
  }
}

#endif /* MATTERNCOLOUR_H_ */
