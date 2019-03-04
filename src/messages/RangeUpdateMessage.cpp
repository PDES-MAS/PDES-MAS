#include "RangeUpdateMessage.h"

using namespace pdesmas;

RangeUpdateMessage::RangeUpdateMessage(){
  RegisterClass(GetType(), &CreateInstance);
}

RangeUpdateMessage:: ~RangeUpdateMessage(){
  // Empty deconstructor
}

pdesmasType RangeUpdateMessage::GetType() const {
  return RANGEUPDATEMESSAGE;
}

AbstractMessage* RangeUpdateMessage::CreateInstance() {
  return new RangeUpdateMessage;
}

void RangeUpdateMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fRange;
  pOstream << DELIM_RIGHT;
}

void RangeUpdateMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fTimestamp;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fRange;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
