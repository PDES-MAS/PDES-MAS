#ifndef ABSTRACTVALUE_H
#define ABSTRACTVALUE_H

#include <iostream>
#include "Serialisable.h"
#include "Types.h"

using namespace std;

namespace pdesmas {
  class AbstractValue: public Serialisable {
    protected:
      virtual int CompareTo(const AbstractValue&) const=0;

    public:
      AbstractValue();
      ~AbstractValue();

      virtual AbstractValue* Create() const=0;
      virtual AbstractValue* Clone() const=0;
      virtual pdesmasType GetType() const=0;

      virtual string GetValueString() const=0;
      virtual void SetValue(string)=0;

      virtual void Serialise(ostream&) const=0;
      virtual void Deserialise(istream&)=0;

      virtual void RegisterWithAbstract()=0;
      virtual AbstractValue* RecreateObject(pdesmasType)=0;

      virtual int Compare(const AbstractValue&) const;
      bool operator ==(const AbstractValue&);
      bool operator !=(const AbstractValue&);
      bool operator >(const AbstractValue&);
      bool operator <(const AbstractValue&);
      bool operator >=(const AbstractValue&);
      bool operator <=(const AbstractValue&);
  };
}
#endif // ABSTRACTVALUE_H
