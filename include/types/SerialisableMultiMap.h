/*
 * SerialisableMultiMap.h
 *
 *  Created on: 15 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef SERIALISABLEMULTIMAP_H_
#define SERIALISABLEMULTIMAP_H_

#include <map>
#include "Serialisable.h"

namespace pdesmas {
  template<typename keyType, typename valueType>
  class SerialisableMultiMap : public multimap<keyType, valueType>, public Serialisable {
    public:
      SerialisableMultiMap();
      virtual ~SerialisableMultiMap();

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };

  template<typename keyType, typename valueType>
  SerialisableMultiMap<keyType, valueType>::SerialisableMultiMap() {

  }
  template<typename keyType, typename valueType>
  SerialisableMultiMap<keyType, valueType>::~SerialisableMultiMap() {

  }
  template<typename keyType, typename valueType>
  void SerialisableMultiMap<keyType, valueType>::Serialise(ostream& pOstream) const {
    const unsigned int size = this->size();
    pOstream << size;
    typename multimap<keyType, valueType>::const_iterator iter;
    for (iter = this->begin(); iter != this->end(); ++iter) {
      pOstream << DELIM_LIST_LEFT << iter->first << DELIM_LIST_LEFT << iter->second << DELIM_LIST_RIGHT;
    }
  }

  template<typename keyType, typename valueType>
  void SerialisableMultiMap<keyType, valueType>::Deserialise(istream& pIstream) {
    this->clear();
    unsigned int size;
    pIstream >> size;
    for (unsigned int counter = 0; counter < size; counter++) {
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


#endif /* SERIALISABLEMULTIMAP_H_ */
