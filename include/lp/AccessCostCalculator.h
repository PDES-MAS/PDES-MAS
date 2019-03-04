#ifndef _ACCESSCOSTCALCULATOR_HPP_
#define _ACCESSCOSTCALCULATOR_HPP_

#include <iostream>
#include "Router.h"
#include "Lp.h"
using namespace std;

namespace pdesmas {

#define THRESHOLD_CONST 1

  class AccessCostCalculator {
    private:
      bool fIsParentCLP;
      unsigned long fPortThreshold;
      unsigned long fLPThreshold;
      unsigned long fCLPLoad;
      list<SsvId> fSSVList;
      typedef map<Direction, unsigned long> CounterMap;
      map<SsvId, CounterMap > fSSVPortAccessMap;
      map<SsvId, CounterMap > fSSVPortHopMap;

      bool ContainsVariable(const SsvId&) const;
      unsigned long GetAccessCount(Direction, const SsvId&) const;
      unsigned long GetHopCount(Direction, const SsvId&) const;
    public:
      AccessCostCalculator(int, int);
      ~AccessCostCalculator();

      void InitialiseCounters(const SsvId&);
      unsigned long UpdateAccessCount(Direction, unsigned long, const SsvId&);
      void RemoveSsvAccessRecord(const SsvId&);
      unsigned long UpdateHopCount(Direction, unsigned long, const SsvId&);
      void RemoveSsvHopRecord(const SsvId&);
      void RemoveSsvFromList(const SsvId&);

      void UpdateLoad(unsigned long, unsigned long, unsigned long, unsigned long);
      bool CheckClpload();
      map<Direction, list<SsvId> > GetMigrationMap();
  };
}
#endif
