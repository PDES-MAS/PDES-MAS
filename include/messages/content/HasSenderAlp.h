/*
 * HasSenderAlp.h
 *
 *  Created on: 16 Sep 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASSENDERALP_H_
#define HASSENDERALP_H_

namespace pdesmas {
  class HasSenderAlp {
    protected:
      unsigned int fSenderAlp;
    public:
      unsigned int GetSenderAlp() const;
      void SetSenderAlp(unsigned int);
  };
}

#endif /* HASSENDERALP_H_ */
