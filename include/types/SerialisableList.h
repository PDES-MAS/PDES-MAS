/*
 * SerialisableList.h
 *
 *  Created on: 15 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef SERIALISABLELIST_H_
#define SERIALISABLELIST_H_

#include <list>
#include "Serialisable.h"

namespace pdesmas {
  template<typename valueType>
  class SerialisableList: public list<valueType> , public Serialisable {
    public:
      SerialisableList();
      virtual ~SerialisableList();

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };

  template<typename valueType>
  SerialisableList<valueType>::SerialisableList() {

  }

  template<typename valueType>
  SerialisableList<valueType>::~SerialisableList() {

  }

  template<typename valueType>
  void SerialisableList<valueType>::Serialise(ostream& pOstream) const {
    const unsigned int size = this->size();
    pOstream << size;
    typename list<valueType>::const_iterator iter;
    for (iter = this->begin(); iter != this->end(); ++iter) {
      pOstream << DELIM_LIST_LEFT << *iter << DELIM_LIST_RIGHT;
    }
  }

  template<typename valueType>
  void SerialisableList<valueType>::Deserialise(istream& pIstream) {
    this->clear();
    unsigned int size;
    pIstream >> size;
    for (unsigned int counter = 0; counter < size; ++counter) {
      pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_LIST_LEFT);
      valueType theValue;
      pIstream >> theValue;
      this->push_back(theValue);
      pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_LIST_RIGHT);
    }
  }
}

#endif /* SERIALISABLELIST_H_ */
