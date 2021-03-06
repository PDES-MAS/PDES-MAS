#include "RangeQueryAntiMessage.h"

using namespace pdesmas;

RangeQueryAntiMessage::RangeQueryAntiMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

RangeQueryAntiMessage::~RangeQueryAntiMessage() {
  // Empty deconstructor
}

pdesmasType RangeQueryAntiMessage::GetType() const {
  return RANGEQUERYANTIMESSAGE;
}

AbstractMessage* RangeQueryAntiMessage::CreateInstance() {
  return new RangeQueryAntiMessage;
}

void RangeQueryAntiMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
  pOstream << DELIM_VAR_SEPARATOR << fRollbackTag;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_VAR_SEPARATOR << fRange;
  pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
  pOstream << DELIM_RIGHT;
}

void RangeQueryAntiMessage::Deserialise(istream& pIstream) {
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
  pIstream >> fRollbackTag;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> original_agent_;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fRange;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fIdentifier;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
