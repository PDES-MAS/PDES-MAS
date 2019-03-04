/*
 * Direction.h
 *
 *  Created on: 17 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

#include <assert.h>

namespace pdesmas {
#ifdef RANGE_QUERIES
  enum Direction {
    HERE=0, UP, LEFT, RIGHT, DIRECTION_SIZE
  };

  inline Direction &operator++(Direction &c) {
    assert(c != DIRECTION_SIZE);
    c = static_cast<Direction>(c + 1);
    return c;
  }

  inline Direction operator++(Direction &c, int) {
    assert(c != DIRECTION_SIZE);
    ++c;
    return static_cast<Direction>(c - 1);
  }
#endif
}
#endif /* DIRECTION_H_ */
