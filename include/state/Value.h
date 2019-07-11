#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <typeinfo>
#include <string>
#include <limits>
#include <sstream>

#include "ObjectMgr.h"
#include "AbstractValue.h"
#include "Helper.h"
#include "Log.h"
#include "Point.h"
#include <spdlog/spdlog.h>

using namespace std;

namespace pdesmas {
  template<typename valueType>
  class Value : public AbstractValue {
  private:
    valueType fValueData;
    pdesmasType fType;

    void InitDescription();

  protected:
    int CompareTo(const AbstractValue &) const;

  public:
    Value();

    Value(const valueType &);

    Value(const Value<valueType> &);

    ~Value();

    AbstractValue *Create() const;

    AbstractValue *Clone() const;

    pdesmasType GetType() const;

    valueType GetValue() const;

    string GetValueString() const;

    void SetValue(string);

    void Serialise(ostream &) const;

    void Deserialise(istream &);

    virtual void RegisterWithAbstract();

    virtual AbstractValue *RecreateObject(pdesmasType);

    static AbstractValue *createValue();

    operator const valueType &() const;

    Value &operator=(const Value<valueType> &);
  };

  template<typename valueType>
  Value<valueType>::Value() :
      fValueData() {
    InitDescription();
    RegisterWithAbstract();
  }

  template<typename valueType>
  Value<valueType>::Value(const valueType &pValueData) :
      fValueData(pValueData) {
    InitDescription();
    RegisterWithAbstract();
  }

  template<typename valueType>
  Value<valueType>::Value(const Value<valueType> &pValue) {
    InitDescription();
    fValueData = pValue.fValueData;
  }

  template<typename valueType>
  Value<valueType>::~Value() {
    // Empty
  }

  template<typename valueType>
  AbstractValue *Value<valueType>::Create() const {
    return new Value();
  }

  template<typename valueType>
  AbstractValue *Value<valueType>::Clone() const {
    return new Value(*this);
  }

  template<typename valueType>
  pdesmasType Value<valueType>::GetType() const {
    return fType;
  }

  template<typename valueType>
  valueType Value<valueType>::GetValue() const {
    return fValueData;
  }

  template<typename valueType>
  string Value<valueType>::GetValueString() const {
    return Helper::string_cast<valueType>(fValueData);
  }

  template<typename valueType>
  void Value<valueType>::SetValue(string pValue) {
    fValueData = Helper::stream_cast<valueType, string>(pValue);
  }

  template<typename valueType>
  void Value<valueType>::Serialise(ostream &pOstream) const {
    pOstream << DELIM_LEFT << fType;
    pOstream << DELIM_VAR_SEPARATOR << fValueData;
    pOstream << DELIM_RIGHT;
  }

  template<typename valueType>
  void Value<valueType>::Deserialise(istream &pIstream) {
    IgnoreTo(pIstream, DELIM_LEFT);
    pIstream >> fType;
    IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
    pIstream >> fValueData;
    IgnoreTo(pIstream, DELIM_RIGHT);
  }

  template<typename valueType>
  void Value<valueType>::RegisterWithAbstract() {
    valueClassMap->RegisterObject(fType, (AbstractValue *(*)()) createValue);
  }

  template<typename valueType>
  AbstractValue *Value<valueType>::RecreateObject(pdesmasType pType) {
    return (valueClassMap->CreateObject(pType));
  }

  template<typename valueType>
  AbstractValue *Value<valueType>::createValue() {
    return new Value();
  }

  template<typename valueType>
  Value<valueType>::operator const valueType &() const {
    return fValueData;
  }

  template<typename valueType>
  Value<valueType> &Value<valueType>::operator=(const Value<valueType> &pValue) {
    fValueData = pValue.fValueData;
    fType = pValue.fType;
    return (*this);
  }

  template<typename valueType>
  int Value<valueType>::CompareTo(const AbstractValue &pAbstractValueAddress) const {
    const Value<valueType> &valueAddress =
        dynamic_cast<const Value<valueType> &> (pAbstractValueAddress);
    return ((fValueData == valueAddress.fValueData) && (fType
                                                        == valueAddress.fType));
  }

  template<typename valueType>
  void Value<valueType>::InitDescription() {
    if (typeid(int) == typeid(valueType)) {
      fType = VALUEINT;
    } else if (typeid(double) == typeid(valueType)) {
      fType = VALUEDOUBLE;
    } else if (typeid(Point) == typeid(valueType)) {
      fType = VALUEPOINT;
    } else if (typeid(string) == typeid(valueType)) {
      fType = VALUESTRING;
    } else if (typeid(long) == typeid(valueType)) {
      fType = VALUELONG;
    } else {

      spdlog::error("Value::InitDescription# Unknown type encountered while constructing value! name: {0}",
                    typeid(valueType).name());
      exit(1);
    }
  }
}
#endif // VALUE_H
