/*
 * HasIDLVTMap.h
 *
 *  Created on: 31 Jan 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef HASIDLVTMap_H_
#define HASIDLVTMap_H_

#include <map>

using namespace std;

namespace pdesmas {
  class HasIDLVTMap {
    private:
      long fLastAgentID;
      map<long, unsigned long> fAgentIdLvtMap;

      void SetAgentLVT(long, unsigned long);
    protected:
      long GetLastLVTAgentID() const;
      void SetAgentReadLVT(long, unsigned long);
      void SetAgentWriteLVT(long, unsigned long);
    public:
      bool HasAgentID(long) const;
      unsigned long GetAgentLVT(long) const;
      void RollbackAgentLVT(long, unsigned long);
      unsigned long GetLVT() const;
  };
}

#endif /* HASIDLVTMap_H_ */
