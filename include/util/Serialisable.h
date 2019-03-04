/*
 * SerialisableList.h
 *
 *  Created on: 17 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef SERIALISABLE_H
#define SERIALISABLE_H

#include <iostream>
#include <limits>
using std::ostream;
using std::istream;
using std::numeric_limits;
using std::streamsize;

namespace pdesmas {

#define DELIM_VAR_SEPARATOR ':'
#define DELIM_LEFT '<'
#define DELIM_RIGHT '>'
#define DELIM_SEPARATORS ":><"
#define DELIM_LIST_LEFT '|'
#define DELIM_LIST_RIGHT '@'

  class Serialisable {
    public:
      Serialisable() {}
      virtual ~Serialisable() {}

      virtual void Serialise(ostream&) const = 0;
      virtual void Deserialise(istream&) = 0;

      istream& IgnoreTo(istream& pIstream, char pChar) {
        pIstream.ignore(numeric_limits<streamsize>::max(), pChar);
        return pIstream;
      }
  };

  inline ostream& operator<<(ostream& pOstream, Serialisable const& pSerialisableObject) {
    pSerialisableObject.Serialise(pOstream);
    return pOstream;
  }

  inline istream& operator>>(istream& pIstream, Serialisable& pSerialisableObject) {
    pSerialisableObject.Deserialise(pIstream);
    return pIstream;
  }
}

#endif // SERIALISABLE_H
