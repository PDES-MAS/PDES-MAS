/*
 * HasNumberOfTraverseHops.h
 *
 *  Created on: 22 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASNUMBEROFTRAVERSEHOPS_H_
#define HASNUMBEROFTRAVERSEHOPS_H_

namespace pdesmas {
  class HasNumberOfTraverseHops {
    protected:
      unsigned int fNumberOfTraverseHops;
    public:
      unsigned int GetNumberOfTraverseHops() const;
      void SetNumberOfTraverseHops(unsigned int);
      void IncrementNumberOfTraverseHops();
  };
}

#endif /* HASTRHOPS_H_ */
