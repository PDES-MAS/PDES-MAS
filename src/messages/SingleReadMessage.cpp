#include "SingleReadMessage.h"

using namespace pdesmas;

SingleReadMessage::SingleReadMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

SingleReadMessage::~SingleReadMessage() {
  // Empty deconstructor
}

pdesmasType SingleReadMessage::GetType() const {
  return SINGLEREADMESSAGE;
}

AbstractMessage* SingleReadMessage::CreateInstance() {
  return new SingleReadMessage;
}

void SingleReadMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
  pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_VAR_SEPARATOR << fSsvId;
  pOstream << DELIM_RIGHT;
}

void SingleReadMessage::Deserialise(istream& pIstream) {
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
  pIstream >> fSsvId;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
