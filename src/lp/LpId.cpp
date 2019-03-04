#include "LpId.h"

using namespace std;
using namespace pdesmas;

LpId::LpId() {
  // Do nothing
}

LpId::LpId(unsigned long pIdentifier, unsigned int pRank) {
  fIdentifier = pIdentifier;
  fRank = pRank;
}

LpId::LpId(const LpId& pLpId) {
  fIdentifier = pLpId.fIdentifier;
  fRank = pLpId.fRank;
}

LpId::~LpId() {
  // Do nothing
}

unsigned long LpId::GetId() const {
  return fIdentifier;
}

unsigned int LpId::GetRank() const {
  return fRank;
}

bool LpId::operator==(LpId const& lp) const {
  return (fIdentifier == lp.fIdentifier && fRank == lp.fRank);
}

bool LpId::operator!=(LpId const& lp) const {
  return (fIdentifier != lp.fIdentifier && fRank != lp.fRank);
}

bool LpId::operator>(LpId const& lp) const {
  return (fIdentifier > lp.fIdentifier && fRank > lp.fRank);
}

bool LpId::operator<(LpId const& lp) const {
  return (fIdentifier < lp.fIdentifier && fRank < lp.fRank);
}

bool LpId::operator>=(LpId const& lp) const {
  return (fIdentifier >= lp.fIdentifier && fRank >= lp.fRank);
}

bool LpId::operator<=(LpId const& lp) const {
  return (fIdentifier <= lp.fIdentifier && fRank <= lp.fRank);
}

bool LpId::operator=(int num) {
  return ((fIdentifier = num) && (fRank = num));
}

void LpId::Serialise(ostream& ostr) const {
  ostr << DELIM_LEFT << fIdentifier << DELIM_VAR_SEPARATOR << fRank << DELIM_RIGHT;
}

void LpId::Deserialise(istream& istr) {
  istr.ignore(numeric_limits<streamsize>::max(), DELIM_LEFT);
  istr >> fIdentifier;
  istr.ignore(numeric_limits<streamsize>::max(), DELIM_VAR_SEPARATOR);
  istr >> fRank;
  istr.ignore(numeric_limits<streamsize>::max(), DELIM_RIGHT);
}
