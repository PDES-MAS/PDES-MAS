/*
 * HasDestination.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASDESTINATION_H_
#define HASDESTINATION_H_

namespace pdesmas {
  class HasDestination {
    protected:
      unsigned int fDestination;
    public:
      unsigned int GetDestination() const;
      void SetDestination(unsigned int);
  };
}

#endif /* HASDESTINATION_H_ */
