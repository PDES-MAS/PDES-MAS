#include "SingleReadResponseMessage.h"
#include "ObjectMgr.h"
#include "Log.h"

using namespace pdesmas;

SingleReadResponseMessage::SingleReadResponseMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

SingleReadResponseMessage::~SingleReadResponseMessage() {
  // Empty deconstructor
}

pdesmasType SingleReadResponseMessage::GetType() const {
  return SINGLEREADRESPONSEMESSAGE;
}

AbstractMessage* SingleReadResponseMessage::CreateInstance() {
  return new SingleReadResponseMessage;
}

void SingleReadResponseMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fTimestamp;
  pOstream << DELIM_VAR_SEPARATOR << fMatternColour;
  pOstream << DELIM_VAR_SEPARATOR << fIdentifier;
  pOstream << DELIM_VAR_SEPARATOR << original_agent_;
  pOstream << DELIM_VAR_SEPARATOR << *fValue;
  pOstream << DELIM_RIGHT;
}

void SingleReadResponseMessage::Deserialise(istream& pIstream) {
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
  string valueString;
  pIstream >> valueString;
  string value = GetValueString(valueString);
  fValue = valueClassMap->CreateObject(GetTypeID(valueString));
  fValue->SetValue(value);
  IgnoreTo(pIstream, DELIM_RIGHT);
}
