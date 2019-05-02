#include "SsvId.h"

using namespace std;
using namespace pdesmas;

SsvId::SsvId() : variable_id_(0) {
  // Empty constructor
}

SsvId::SsvId(int variable_id) {
  variable_id_ = variable_id;
}

SsvId::~SsvId() {
  // Empty deconstructor
}


bool SsvId::operator==(SsvId const &a_ssv_id) const {
  return variable_id_ == a_ssv_id.variable_id_;
}

SsvId &SsvId::operator=(SsvId const &a_ssv_id) {
  variable_id_ = a_ssv_id.variable_id_;
  return *this;
}

bool SsvId::operator<(SsvId const &a_ssv_id) const {
  return variable_id_ < a_ssv_id.variable_id_;
}

void SsvId::Serialise(ostream &pOstream) const {
  pOstream << DELIM_LEFT;
  pOstream << variable_id_;
  pOstream << DELIM_RIGHT;
}

void SsvId::Deserialise(istream &pIstream) {
  IgnoreTo(pIstream, DELIM_LEFT);
  pIstream >> variable_id_;
  IgnoreTo(pIstream, DELIM_RIGHT);
}
