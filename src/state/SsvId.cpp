#include "SsvId.h"

using namespace std;
using namespace pdesmas;

SsvId::SsvId() : fObjectId(0), fVariableId(0) {
  // Empty constructor
}

SsvId::SsvId(long pObjectId, int pVariableId) {
  fObjectId = pObjectId;
  fVariableId = pVariableId;
}

SsvId::~SsvId() {
  // Empty deconstructor
}

long SsvId::GetObjectId() const {
  return fObjectId;
}

bool SsvId::operator==(SsvId const& pSsvId) const {
  return (fObjectId == pSsvId.fObjectId) && (fVariableId == pSsvId.fVariableId);
}

SsvId& SsvId::operator=(SsvId const& pSsvId) {
  fObjectId = pSsvId.fObjectId;
  fVariableId = pSsvId.fVariableId;
  return *this;
}

bool SsvId::operator<(SsvId const& pSsvId) const {
  if (fObjectId < pSsvId.fObjectId) return true;
  if ((fObjectId == pSsvId.fObjectId) && (fVariableId < pSsvId.fVariableId)) return true;
  return false;
}

void SsvId::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << fObjectId;
  pOstream << DELIM_VAR_SEPARATOR << fVariableId;
  pOstream << DELIM_RIGHT;
}

void SsvId::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_LEFT);
  pIstream >> fObjectId;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fVariableId;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
