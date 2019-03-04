#ifndef _SSVID_H_
#define _SSVID_H_

#include "Serialisable.h"

using namespace std;

namespace pdesmas {
  class SsvId: public Serialisable {
    private:
      long fObjectId;
      int fVariableId;

    public:
      SsvId();
      SsvId(long, int);
      ~SsvId();

      long GetObjectId() const;

      bool operator==(SsvId const&) const;
      SsvId& operator=(SsvId const&);
      bool operator<(SsvId const&) const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}

#endif

