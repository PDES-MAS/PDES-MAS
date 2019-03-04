/*
 * Log.h
 *
 *  Created on: 12 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef _PAIR_H_
#define _PAIR_H_

#include <climits>
#include <limits>
#include "Serialisable.h"

using std::ostream;
using std::istream;
using std::numeric_limits;
using std::streamsize;

namespace pdesmas {
  template<typename ValueType>
  class Pair: public Serialisable {
    protected:
      ValueType fX;
      ValueType fY;
    public:
      Pair(){};
      Pair(ValueType, ValueType);
      Pair(const Pair<ValueType>&);
      ~Pair();

      ValueType GetX() const;
      ValueType GetY() const;
      void SetX(ValueType const);
      void SetY(ValueType const);

      bool operator==(Pair<ValueType> const&) const;
      bool operator!=(Pair<ValueType> const&) const;
      bool operator<(Pair<ValueType> const&) const;
      bool operator>(Pair<ValueType> const&) const;
      Pair<ValueType>& operator=(Pair<ValueType> const&);
      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };

  template<typename ValueType>
  Pair<ValueType>::Pair(ValueType pX, ValueType pY) :
    fX(pX), fY(pY) {
  }

  template<typename ValueType>
  Pair<ValueType>::Pair(const Pair<ValueType>& pPair) : fX(pPair.fX), fY(pPair.fY) {
  }

  template<typename ValueType>
  Pair<ValueType>::~Pair() {
  }

  template<typename ValueType>
  ValueType Pair<ValueType>::GetX() const {
    return this->fX;
  }

  template<typename ValueType>
  ValueType Pair<ValueType>::GetY() const {
    return this->fY;
  }

  template<typename ValueType>
  void Pair<ValueType>::SetX(ValueType const pValue) {
    this->fX = pValue;
  }

  template<typename ValueType>
  void Pair<ValueType>::SetY(ValueType const pValue) {
    this->fY = pValue;
  }

  template<typename ValueType>
  bool Pair<ValueType>::operator==(Pair<ValueType> const& pPair) const {
    return ((this->fX == pPair.fX) && (this->fY == pPair.fY));
  }

  template<typename ValueType>
  bool Pair<ValueType>::operator!=(Pair<ValueType> const& pPair) const {
    return ((this->fX != pPair.fX) || (this->fY != pPair.fY));
  }

  template<typename ValueType>
  Pair<ValueType>& Pair<ValueType>::operator=(Pair<ValueType> const& pPair) {
    this->fX = pPair.fX;
    this->fY = pPair.fY;
    return *this;
  }

  template<typename ValueType>
  bool Pair<ValueType>::operator<(Pair<ValueType> const& pPair) const {
    return ((this->fX <= pPair.fX) && (this->fY <= pPair.fY));
  }

  template<typename ValueType>
  bool Pair<ValueType>::operator>(Pair<ValueType> const& pPair) const {
    return ((this->fX >= pPair.fX) && (this->fY >= pPair.fY));
  }

  template<typename ValueType>
  void Pair<ValueType>::Serialise(ostream& pOstream) const {
    pOstream << DELIM_LEFT << this->fX << DELIM_VAR_SEPARATOR << this-> fY << DELIM_RIGHT;
  }

  template<typename ValueType>
  void Pair<ValueType>::Deserialise(istream& pIstream) {
    pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_LEFT);
    pIstream >> fX;
    pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_VAR_SEPARATOR);
    pIstream >> fY;
    pIstream.ignore(numeric_limits<streamsize>::max(), DELIM_RIGHT);
  }
}
#endif
