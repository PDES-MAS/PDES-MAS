#include "GvtValueMessage.h"

using namespace pdesmas;

GvtValueMessage::GvtValueMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

GvtValueMessage::~GvtValueMessage() {
  // Empty deconstructor
}

pdesmasType GvtValueMessage::GetType() const {
  return GVTVALUEMESSAGE;
}

AbstractMessage* GvtValueMessage::CreateInstance() {
  return new GvtValueMessage;
}

void GvtValueMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fGVT;
  pOstream << DELIM_RIGHT;
}

void GvtValueMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fGVT;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
