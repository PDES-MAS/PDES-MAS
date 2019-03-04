/*
 * Log.h
 *
 *  Created on: 12 Mar 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#include "Range.h"

using namespace pdesmas;

Range::Range() {
}

Range::Range(Point pMinValue, Point pMaxValue) {
  this->fX = pMinValue;
  this->fY = pMaxValue;
}

Range::Range(const Range& pRange) {
  this->fX = pRange.fX;
  this->fY = pRange.fY;
}

Range::~Range() {
}

void Range::SetMinRangeValue(Point const pMinValue) {
  this->fX = pMinValue;
}

void Range::SetMaxRangeValue(Point const pMaxValue) {
  this->fY = pMaxValue;
}

Point Range::GetMinRangeValue() const {
  return this->fX;
}

Point Range::GetMaxRangeValue() const {
  return this->fY;
}

bool Range::IsRangeOverlapping(Range const pRange) const {
  Point rangeBottom1 = pRange.fX;
  Point rangeTop2 = pRange.fY;
  Point rangeTop1 = Point (rangeBottom1.GetX(), rangeTop2.GetY());
  Point rangeBottom2 = Point (rangeTop2.GetX(), rangeBottom1.GetY());

  Point thisBottom1 = this->fX;
  Point thisTop2 = this->fY;
  Point thisTop1 = Point (thisBottom1.GetX(), thisTop2.GetY());
  Point thisBottom2 = Point (thisTop2.GetX(), thisBottom1.GetY());

  if ((IsValueOverlapping(rangeBottom1)) || (IsValueOverlapping(rangeBottom2))
      || (IsValueOverlapping(rangeTop1)) || (IsValueOverlapping(rangeTop2))) return true;
  if ((pRange.IsValueOverlapping(thisBottom1)) || (pRange.IsValueOverlapping(
      thisBottom2)) || (pRange.IsValueOverlapping(thisTop1))
      || (pRange.IsValueOverlapping(thisTop2))) return true;
  return false;
}

bool Range::IsValueOverlapping(Point const pValue) const {
  return ((pValue < this->fY) && (pValue > this->fX));
}

void Range::MinMaxofTwoRanges(Range const pRange) {
  this->fX.Min(pRange.fX);
  this->fY.Max(pRange.fY);
}
