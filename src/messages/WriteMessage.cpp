#include "WriteMessage.h"
#include "ObjectMgr.h"

using namespace pdesmas;

WriteMessage::WriteMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

WriteMessage::~WriteMessage() {
  // Empty deconstructor
}

pdesmasType WriteMessage::GetType() const {
  return WRITEMESSAGE;
}

AbstractMessage* WriteMessage::CreateInstance() {
  return new WriteMessage;
}

void WriteMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fNumberOfHops;
  pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_VAR_SEPARATOR << fSsvId;
  pOstream << DELIM_VAR_SEPARATOR << *fValue;
  pOstream << DELIM_RIGHT;
}

void WriteMessage::Deserialise(istream& pIstream) {
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
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  string valueString;
  pIstream >> valueString;
  string value = GetValueString(valueString);
  fValue = valueClassMap->CreateObject(GetTypeID(valueString));
  fValue->SetValue(value);
  IgnoreTo(pIstream, DELIM_RIGHT);
}
