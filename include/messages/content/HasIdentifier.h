/*
 * HasIdentifier.h
 *
 *  Created on: 28 Jan 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASIDENTIFIER_H_
#define HASIDENTIFIER_H_

namespace pdesmas {
  class HasIdentifier {
    protected:
      unsigned long fIdentifier;
    public:
      unsigned long GetIdentifier() const;
      void SetIdentifier(unsigned long);
  };
}

#endif /* HASIDENTIFIER_H_ */
