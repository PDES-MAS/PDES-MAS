/*
 * HasOrigin.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASORIGIN_H_
#define HASORIGIN_H_

namespace pdesmas {
  class HasOrigin {
    protected:
      unsigned int fOrigin;
    public:
      unsigned int GetOrigin() const;
      void SetOrigin(unsigned int);
  };
}

#endif /* HASORIGIN_H_ */
