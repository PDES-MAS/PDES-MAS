#ifndef _GVT_CALCULATOR_H_
#define _GVT_CALCULATOR_H_

/**
 *\brief Class used for GVT calculation (uses Mattern's)
 *
 *GvtCalculator.h
 * 
 *Author: Michael Lees (mhl) 
 *Date: (08/02/05)
 *
 *
 *Description: This is a class which maintains gvt values on an LP. Each
 *Lp in the simulation has a GvtCalculator which is responsible for
 *keeping the Lps gvt value uptodate. The algorithm used to perform GVT
 *by this Calculator is matterns, for descriptions of what all the time
 *values mean please read the paper on matterns GVT.  It should be
 *possible to implement another GVT calculator as long as the same
 *public methods are provided.
 *
 */

#include <map>
#include "GvtMessage.h"
#include "MatternColour.h"
#include "GvtValueMessage.h"
#include "GvtControlMessage.h"
#include "GvtRequestMessage.h"

using namespace std;

namespace pdesmas {
  class Lp;
  class GvtCalculator {
    private:
      Lp* fLp;
      MatternColour fColour;
      unsigned long fRedTime;
      bool fIsGVTStarter;
      map<unsigned int, long> fWhiteTransientMessageCounter;
      unsigned int fNextLpInRing;
      bool fIsAcceptingRequests;

      void ProcessFirstCutGvtControl(const GvtControlMessage*);
      void ProcessSecondCutGvtControl(const GvtControlMessage*);
      void SendAndSetGvt(const GvtControlMessage*);

    public:
      GvtCalculator();
      GvtCalculator(Lp*);
      ~GvtCalculator();

      void ProcessMessage(const GvtRequestMessage*);
      void ProcessMessage(const GvtValueMessage*);
      void ProcessMessage(const GvtControlMessage*);

      MatternColour GetColour() const;
      void SetRedTime(unsigned long);
      unsigned long GetRedTime() const;
      long GetWhiteTransientMessageCounter(unsigned int) const;
      void DecrementWhiteTransientMessageCounter(unsigned int);
      void IncrementWhiteTransientMessageCounter(unsigned int);
  };
}
#endif
