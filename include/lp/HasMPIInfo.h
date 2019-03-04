/*
 * HasMPIInfo.h
 *
 *  Created on: 31 Jan 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef HASMPIInfo_H_
#define HASMPIInfo_H_

namespace pdesmas {
  class HasMPIInfo {
    private:
      unsigned int fRank;
      unsigned int fSize;
      unsigned int fNumberOfClps;
      unsigned int fNumberOfAlps;
    public:
      unsigned int GetRank() const {
        return fRank;
      }

      void SetRank(unsigned int pRank) {
        fRank = pRank;
      }

      unsigned int GetSize() const {
        return fSize;
      }

      void SetSize(unsigned int pSize) {
        fSize = pSize;
      }

      unsigned int GetNumberOfClps() const {
        return fNumberOfClps;
      }

      void SetNumberOfClps(unsigned int pNumberOfClps) {
        fNumberOfClps = pNumberOfClps;
      }

      unsigned int GetNumberOfAlps() const {
        return fNumberOfAlps;
      }

      void SetNumberOfAlps(unsigned int pNumberOfAlps) {
        fNumberOfAlps = pNumberOfAlps;
      }
  };
}
#endif /* HASMPIInfo_H_ */
