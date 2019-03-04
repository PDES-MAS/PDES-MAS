#ifndef _LPID_H_
#define _LPID_H_

/**
 * \brief Class used to identify Lps with the Simulation
 *
 *LpId.h
 *
 *Author: Michael Lees (mhl) 
 *Date: (19/01/05)
 *
 *
 *Description: Uniquely identifies an Lp in the system in two
 *ways. First there is the Id which is a kernel identifier. The LpId
 *also has a communication identifier which may change with different
 *communication infrastructures. Here it is known as rank as it is in
 *MPI which is the default comm infrastructure. At the moment for
 *simplicity the Id and the rank take the same value.
 *
 *Revisions:
 *mhl - 28/01/05 - changed < to be const member func, Added serialise methods
 *
 */

#include "Serialisable.h"

using namespace std;

namespace pdesmas {
  class LpId: public Serialisable {
    protected:
      unsigned long fIdentifier;
      unsigned int fRank;

    public:
      LpId();
      LpId(unsigned long, unsigned int);
      LpId(const LpId&);
      ~LpId();

      unsigned long GetId() const;
      unsigned int GetRank() const;

      bool operator ==(LpId const& lp) const;
      bool operator !=(LpId const& lp) const;
      bool operator >(LpId const& lp) const;
      bool operator <(LpId const& lp) const;
      bool operator >=(LpId const& lp) const;
      bool operator <=(LpId const& lp) const;
      bool operator=(int num);

      void Serialise(ostream& ostr) const;
      void Deserialise(istream& istr);
  };

}
#endif
