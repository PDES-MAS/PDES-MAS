/*
 * HasStateVariableMap.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASSTATEVARIABLEMAP_H_
#define HASSTATEVARIABLEMAP_H_

#include "SerialisableMap.h"
#include "SsvId.h"
#include "StateVariable.h"

namespace pdesmas {
  class HasStateVariableMap {
    protected:
      SerialisableMap<SsvId, StateVariable > fStateVariableMap;
    public:
      const SerialisableMap<SsvId, StateVariable >& GetStateVariableMap() const;
      void SetStateVariableMap(const SsvId&, const StateVariable&);
  };
}

#endif /* HASSTATEVARIABLEMAP_H_ */
