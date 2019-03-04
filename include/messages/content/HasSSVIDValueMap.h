/*
 * HasSSVIDValueMap.h
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASSSVIDVALUEMAP_H_
#define HASSSVIDVALUEMAP_H_

#include "SerialisableMap.h"
#include "SsvId.h"
#include "Value.h"
#include "Point.h"

namespace pdesmas {
  class HasSSVIDValueMap {
    protected:
      SerialisableMap<SsvId, Value<Point> > fSsvIdValueMap;
    public:
      const SerialisableMap<SsvId, Value<Point> >& GetSsvValueList() const;
      void SetSsvValueList(const SerialisableMap<SsvId, Value<Point> >&);
      void SetValueForVariable(const SsvId&, const Value<Point>&);
      void ClearSsvList();
  };
}

#endif /* HASSSVIDVALUEMAP_H_ */
