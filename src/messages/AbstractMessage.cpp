#include "AbstractMessage.h"
#include "ObjectMgr.h"

using namespace pdesmas;

AbstractMessage::AbstractMessage() {
  // Empty constructor
}

AbstractMessage::~AbstractMessage() {
  // Empty deconstructor
}

void AbstractMessage::RegisterClass(pdesmasType pType, AbstractMessage* (*pCreateInstance)()) {
  messageClassMap->RegisterObject(pType, pCreateInstance);
}

AbstractMessage* AbstractMessage::RecreateInstance(pdesmasType pType) {
  return messageClassMap->CreateObject(pType);
}
