/*
 * HasValue.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASVALUE_H_
#define HASVALUE_H_

#include "AbstractValue.h"

namespace pdesmas {
  class HasValue {
    protected:
      AbstractValue* fValue;
    public:
      const AbstractValue* GetValue() const;
      void SetValue(AbstractValue*);
      void ClearValue();
  };
}

#endif /* HASVALUE_H_ */
