/*
 * Point.cpp
 *
 *  Created on: 12 Mar 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include "Point.h"

using namespace pdesmas;

Point::Point() {
  this->fX = 0;
  this->fY = 0;
}

Point::Point(int pX, int pY) {
  this->fX = pX;
  this->fY = pY;
}

Point::Point(const Point& pPoint) {
  this->fX = pPoint.fX;
  this->fY = pPoint.fY;
}

Point::~Point() {
}

Point& Point::Min(Point const& pPoint) {
  if (pPoint.fX == INT_MAX && pPoint.fY == INT_MAX) return *this;
  if (this->fX == INT_MAX && this->fY == INT_MAX) {
    *this = pPoint;
    return *this;
  }
  if (pPoint.fX < this->fX) this->fX = pPoint.fX;
  if (pPoint.fY < this->fY) this->fY = pPoint.fY;
  return *this;
}

Point& Point::Max(Point const& pPoint) {
  if (pPoint.fX == INT_MAX && pPoint.fY == INT_MAX) return *this;
  if (this->fX == INT_MAX && this->fY == INT_MAX) {
    *this = pPoint;
    return *this;
  }

  if (pPoint.fX > this->fX) this->fX = pPoint.fX;
  if (pPoint.fY > this->fY) this->fY = pPoint.fY;
  return *this;
}
