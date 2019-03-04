#include <typeinfo>
#include <sstream>
#include "AbstractValue.h"

using namespace pdesmas;

AbstractValue::AbstractValue() {
  // Empty
}

AbstractValue::~AbstractValue() {
  // Empty
}

int AbstractValue::Compare(const AbstractValue& absObj) const {
	if (typeid(*this) == typeid(absObj)) {
		return CompareTo(absObj);
	}
	if (typeid(*this).before (typeid(absObj))) {
		return -1;
	}
	return 1;
}

inline bool operator == (AbstractValue const& left, AbstractValue const& right) {
 	return left.Compare(right)==0;
}

inline bool operator != (AbstractValue const& left, AbstractValue const& right) {
 	return left.Compare(right)!=0;
}

inline bool operator > (AbstractValue const& left, AbstractValue const& right) {
 	return left.Compare(right)> 0;
}

inline bool operator >= (AbstractValue const& left, AbstractValue const& right) {
 	return left.Compare(right)>=0;
}

inline bool operator < (AbstractValue const& left, AbstractValue const& right) {
 	return left.Compare(right)< 0;
}

inline bool operator <= (AbstractValue const& left, AbstractValue const& right) {
 	return left.Compare(right)<=0;
}
