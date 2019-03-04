/*
 * HasValue.cpp
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "HasValue.h"

using namespace pdesmas;

const AbstractValue* HasValue::GetValue() const {
  return fValue;
}

void HasValue::SetValue(AbstractValue* pValue) {
  fValue = pValue;
}

void HasValue::ClearValue() {
  if (NULL != fValue) {
    delete fValue;
    fValue = NULL;
  }
}
