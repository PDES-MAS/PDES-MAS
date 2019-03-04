/*
 * HasRedMessageTime.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASREDMESSAGETIME_H_
#define HASREDMESSAGETIME_H_

namespace pdesmas {
  class HasRedMessageTime {
    protected:
      unsigned long fRedMessageTime;
    public:
      unsigned long GetRedMessageTime() const;
      void SetRedMessageTime(unsigned long);
  };
}

#endif /* HASREDMESSAGETIME_H_ */
