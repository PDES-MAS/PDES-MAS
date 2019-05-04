#include "WriteResponseMessage.h"
#include "WriteMessage.h"

using namespace pdesmas;

WriteResponseMessage::WriteResponseMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

WriteResponseMessage::~WriteResponseMessage() {
  // Empty deconstructor
}

pdesmasType WriteResponseMessage::GetType() const {
  return WRITERESPONSEMESSAGE;
}

AbstractMessage* WriteResponseMessage::CreateInstance() {
  return new WriteResponseMessage;
}

void WriteResponseMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_VAR_SEPARATOR << fWriteStatus;
  pOstream << DELIM_RIGHT;
}

void WriteResponseMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fTimestamp;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fMatternColour;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fIdentifier;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> original_agent_;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fWriteStatus;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
