/*
 * Initialisor.h
 *
 *  Created on: 18 Aug 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef INITIALISOR_H_
#define INITIALISOR_H_

#include <string>
#include <list>
#include "ForwardingTable.h"
#include "Direction.h"
#include "Range.h"

using namespace std;

namespace pdesmas {

#define MAX_LINE_LENGTH 80

  class Clp;

  class Initialisor {
    private:
      bool fHasInitInt;
      bool fHasInitLong;
      bool fHasInitDouble;
      bool fHasInitPoint;
      bool fHasInitString;

      Clp* fClp;
      map<unsigned int, Range> fClpIdRangeMap;
      map<unsigned int, list<SsvId> > fClpIdSsvIdMap;
      map<unsigned int, unsigned int> fAlpToClpMap;

      void ParseMessage(const string) const;
      void ParseALP(const string);
      void ParseSSV(const string);
      void ParseSSVForALP(const string);
      void ParseCLP(const string);

      void InitType(const string);

    public:
      Initialisor();
      Initialisor(Clp*);
      ~Initialisor();

      void ParseFileCLP(const string);
      void ParseFileALP(const string);

      const map<unsigned int, Range>& GetClpToRangeMap() const;
      const map<unsigned int, list<SsvId> >& GetClpToSsvMap() const;
      const map<unsigned int, unsigned int>& GetAlpToClpMap() const;
  };
}

#endif /* INITIALISOR_H_ */
