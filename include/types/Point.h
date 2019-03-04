/*
 * Point.h
 *
 *  Created on: 12 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef POINT_H_
#define POINT_H_

#include "Pair.h"

namespace pdesmas {
  class Point: public Pair<int> {
    public:
      Point();
      Point(int, int);
      Point(const Point&);
      virtual ~Point();

      Point& Min(Point const&);
      Point& Max(Point const&);
  };
}

#endif /* POINT_H_ */
