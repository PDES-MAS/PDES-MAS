#include "StateMigrationMessage.h"

using namespace pdesmas;

StateMigrationMessage::StateMigrationMessage() {
  RegisterClass(GetType(), &CreateInstance);
}

StateMigrationMessage::~StateMigrationMessage() {
  // Empty deconstructor
}

pdesmasType StateMigrationMessage::GetType() const {
  return STATEMIGRATIONMESSAGE;
}

AbstractMessage* StateMigrationMessage::CreateInstance() {
  return new StateMigrationMessage;
}

void StateMigrationMessage::Serialise(ostream& pOstream) const {
  pOstream << DELIM_LEFT << GetType();
  pOstream << DELIM_VAR_SEPARATOR << fOrigin;
  pOstream << DELIM_VAR_SEPARATOR << fDestination;
  pOstream << DELIM_VAR_SEPARATOR << fStateVariableMap;
  pOstream << DELIM_RIGHT;
}

void StateMigrationMessage::Deserialise(istream& pIstream) {
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fOrigin;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fDestination;
  IgnoreTo(pIstream, DELIM_VAR_SEPARATOR);
  pIstream >> fStateVariableMap;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
