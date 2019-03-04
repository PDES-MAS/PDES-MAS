/*
 * Log.h
 *
 *  Created on: 12 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef _RANGE_H_
#define _RANGE_H_

#include "Pair.h"
#include "Point.h"

namespace pdesmas {
  class Range: public Pair<Point> {
    public:
      Range();
      Range(Point, Point);
      Range(const Range&);
      ~Range();

      Point GetMinRangeValue() const;
      Point GetMaxRangeValue() const;
      void SetMinRangeValue(Point const);
      void SetMaxRangeValue(Point const);

      bool IsRangeOverlapping(Range const) const;
      bool IsValueOverlapping(Point const) const;
      void MinMaxofTwoRanges(Range const);
  };
}

#endif

