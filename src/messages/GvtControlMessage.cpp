#include "GvtControlMessage.h"

using namespace pdesmas;

GvtControlMessage::GvtControlMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

GvtControlMessage::~GvtControlMessage() {
  // Empty deconstructor
}

pdesmasType GvtControlMessage::GetType() const {
  return GVTCONTROLMESSAGE;
}

AbstractMessage* GvtControlMessage::CreateInstance() {
  return new GvtControlMessage;
}

void GvtControlMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fMessageMinimumTime;
  pOstream << DELIM_VAR_SEPARATOR << fRedMessageTime;
  pOstream << DELIM_VAR_SEPARATOR << fMatternCut;
  pOstream << DELIM_VAR_SEPARATOR << fMessageCount;
  pOstream << DELIM_RIGHT;
}

void GvtControlMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fMessageMinimumTime;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fRedMessageTime;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fMatternCut;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fMessageCount;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
