/*
 * HasMatternColour.cpp
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasMatternColour.h"

using namespace pdesmas;

MatternColour HasMatternColour::GetMatternColour() const {
  return fMatternColour;
}

void HasMatternColour::SetMatternColour(MatternColour pMatternColour) {
  fMatternColour = pMatternColour;
}
