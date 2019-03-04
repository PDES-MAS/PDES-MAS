/*
 * SerialisableMultiMap.h
 *
 *  Created on: 15 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef SERIALISABLEMAP_H_
#define SERIALISABLEMAP_H_

#include <map>
#include "Serialisable.h"
#include "Log.h"

using std::map;
using std::numeric_limits;
using std::streamsize;

namespace pdesmas {
  template<typename keyType, typename valueType>
  class SerialisableMap: public map<keyType, valueType> , public Serialisable {
    public:
      SerialisableMap();
      virtual ~SerialisableMap();

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };

  template<typename keyType, typename valueType>
  SerialisableMap<keyType, valueType>::SerialisableMap() {

  }
  template<typename keyType, typename valueType>
  SerialisableMap<keyType, valueType>::~SerialisableMap() {

  }
  template<typename keyType, typename valueType>
  void SerialisableMap<keyType, valueType>::Serialise(ostream& pOstream) const {
    const unsigned int size = this->size();
    pOstream << size;
    typename map<keyType, valueType>::const_iterator iter;
    for (iter = this->begin(); iter != this->end(); ++iter) {
      pOstream << DELIM_LIST_LEFT << iter->first << DELIM_LIST_LEFT
          << iter->second << DELIM_LIST_RIGHT;
    }
  }

  template<typename keyType, typename valueType>
  void SerialisableMap<keyType, valueType>::Deserialise(istream& pIstream) {
    this->clear();
    unsigned int size;
    pIstream >> size;
    for (unsigned int counter = 0; counter < size; ++counter) {
      pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_LIST_LEFT);
      keyType key;
      pIstream >> key;
      pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_LIST_LEFT);
      valueType value;
      pIstream >> value;
      this->insert(make_pair(key, value));
      pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_LIST_RIGHT);
    }
  }
}

#endif /* SERIALISABLEMAP_H_ */
