/*
 * HasMessageMinimumTime.h
 *
 *  Created on: 21 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef HASMESSAGEMINIMUMTIME_H_
#define HASMESSAGEMINIMUMTIME_H_

namespace pdesmas {
  class HasMessageMinimumTime {
    protected:
      unsigned long fMessageMinimumTime;
    public:
      unsigned long GetMessageMinimumTime() const;
      void SetMessageMinimumTime(unsigned long);
  };
}

#endif /* HASMESSAGEMINIMUMTIME_H_ */
