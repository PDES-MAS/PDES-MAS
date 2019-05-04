#include "RangeQueryMessage.h"

using namespace pdesmas;

RangeQueryMessage::RangeQueryMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

RangeQueryMessage::~RangeQueryMessage() {
  // Empty deconstructor
}

pdesmasType RangeQueryMessage::GetType() const {
  return RANGEQUERYMESSAGE;
}

AbstractMessage* RangeQueryMessage::CreateInstance() {
  return new RangeQueryMessage;
}

void RangeQueryMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
  pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_VAR_SEPARATOR << fRange;
  pOstream << DELIM_VAR_SEPARATOR << fNumberOfTraverseHops;
  pOstream << DELIM_VAR_SEPARATOR << fSsvIdValueMap;
  pOstream << DELIM_RIGHT;
}

void RangeQueryMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fTimestamp;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fMatternColour;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fNumberOfHops;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fIdentifier;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> original_agent_;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fRange;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fNumberOfTraverseHops;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fSsvIdValueMap;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
