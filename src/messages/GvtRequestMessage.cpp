#include "GvtRequestMessage.h"

using namespace pdesmas;

GvtRequestMessage::GvtRequestMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

GvtRequestMessage::~GvtRequestMessage() {
  // Empty deconstructor
}

pdesmasType GvtRequestMessage::GetType() const {
  return GVTREQUESTMESSAGE;
}

AbstractMessage* GvtRequestMessage::CreateInstance() {
  return new GvtRequestMessage;
}

void GvtRequestMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_RIGHT;
}

void GvtRequestMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
