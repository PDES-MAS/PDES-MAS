/*
 * HasNumberOfHops.h
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASNUMBEROFHOPS_H_
#define HASNUMBEROFHOPS_H_

namespace pdesmas {
  class HasNumberOfHops {
    protected:
      unsigned long fNumberOfHops;
    public:
      unsigned long GetNumberOfHops() const;
      void SetNumberOfHops(unsigned long);
      void IncrementNumberOfHops();
  };
}

#endif /* HASNUMBEROFHOPS_H_ */
