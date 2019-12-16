#include "SsvId.h"

using namespace std;
using namespace pdesmas;

SsvId::SsvId() : id_(0) {
  // Empty constructor
}

SsvId::SsvId(unsigned long variable_id) {
  id_ = variable_id;
}

SsvId::~SsvId() {
  // Empty deconstructor
}


bool SsvId::operator==(SsvId const &a_ssv_id) const {
  return id_ == a_ssv_id.id_;
}

SsvId &SsvId::operator=(SsvId const &a_ssv_id) {
  id_ = a_ssv_id.id_;
  return *this;
}

bool SsvId::operator<(SsvId const &a_ssv_id) const {
  return id_ < a_ssv_id.id_;
}

void SsvId::Serialise(ostream &pOstream) const {
  pOstream << DELIM_LEFT;
  pOstream << id_;
  pOstream << DELIM_RIGHT;
}

void SsvId::Deserialise(istream &pIstream) {
  IgnoreTo(pIstream, DELIM_LEFT);
  pIstream >> id_;
  IgnoreTo(pIstream, DELIM_RIGHT);
}

unsigned long SsvId::id() const {
  return id_;
}

unsigned long SsvId::owner() const {
  return owner_;
}

bool SsvId::is_public() const {
  return is_public_;
}
