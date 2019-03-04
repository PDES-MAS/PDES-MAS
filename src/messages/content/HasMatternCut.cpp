/*
 * HasMatternCut.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasMatternCut.h"

using namespace pdesmas;

MatternCut HasMatternCut::GetMatternCut() const {
  return fMatternCut;
}

void HasMatternCut::SetMatternCut(MatternCut pMatternCut) {
  fMatternCut = pMatternCut;
}
