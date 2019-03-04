#include "EndMessage.h"

using namespace pdesmas;

EndMessage::EndMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

EndMessage::~EndMessage() {
  // Empty deconstructor
}

pdesmasType EndMessage::GetType() const {
  return ENDMESSAGE;
}

AbstractMessage* EndMessage::CreateInstance() {
  return new EndMessage;
}

void EndMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fSenderAlp;
  pOstream << DELIM_RIGHT;
}

void EndMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fSenderAlp;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
