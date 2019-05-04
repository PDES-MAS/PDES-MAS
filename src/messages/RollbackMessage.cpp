#include "RollbackMessage.h"

using namespace pdesmas;

RollbackMessage::RollbackMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

RollbackMessage::~RollbackMessage() {
  // Empty deconstructor
}

pdesmasType RollbackMessage::GetType() const {
  return ROLLBACKMESSAGE;
}

AbstractMessage* RollbackMessage::CreateInstance() {
  return new RollbackMessage;
}

void RollbackMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fRollbackTag;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_RIGHT;
}

void RollbackMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fTimestamp;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fMatternColour;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fRollbackTag;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> original_agent_;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
