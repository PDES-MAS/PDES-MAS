#include "RollbackTag.h"
#include <sys/time.h>
#include "Helper.h"
using namespace std;
using namespace pdesmas;

/*************************************
 METHOD: RollbackTag

 DESC: Constructor which initialises both fields of RollbackTag

 ARGS: SsvId& lp - state variable where rollback originated
 unsigned long& t - time of original rollback
 RETURN: none
 **************************************/
RollbackTag::RollbackTag(SsvId const& pSsv, unsigned long pTime, int pType) :
  originalSsv(pSsv), time(pTime), type(pType) {
  realTimeStamp = Helper::GetTimeInMS();
}

/*************************************
 METHOD: operator==

 DESC: returns true if two RollbackTags are equal, that is they
 represent the same rollback

 ARGS: RollbackTag - the tag to compare this one with
 RETURN: bool - true if they represent the same rollback
 **************************************/
bool RollbackTag::operator==(RollbackTag const& rbTag) const {
  return ((originalSsv == rbTag.originalSsv) && (time == rbTag.time) && (type
      == rbTag.type) && (realTimeStamp == rbTag.realTimeStamp));
}

/*************************************
 METHOD: operator<

 DESC: returns true if two RollbackTags are equal, that is they
 represent the same rollback

 ARGS: RollbackTag - the tag to compare this one with
 RETURN: bool - true if they represent the same rollback
 **************************************/
bool RollbackTag::operator<(RollbackTag const& rbTag) const {
  return (time < rbTag.time);
}

/*************************************
 METHOD: operator=

 DESC: Assignment operator

 ARGS: RollbackTag - the tag to compare this one with
 RETURN: bool - true if they represent the same rollback
 **************************************/
void RollbackTag::operator=(RollbackTag const& rbTag) {
  originalSsv = rbTag.originalSsv;
  time = rbTag.time;
  type = rbTag.type;
  realTimeStamp = rbTag.realTimeStamp;
}

/*************************************
 METHOD: serialise

 DESC: Serialises the RollbackTag class into a byte stream

 ARGS: ostream& - the stream to serialise into
 RETURN: void
 **************************************/
void RollbackTag::Serialise(std::ostream& ostr) const {
  ostr << DELIM_LEFT << originalSsv << DELIM_VAR_SEPARATOR << time
      << DELIM_VAR_SEPARATOR << type << DELIM_VAR_SEPARATOR << realTimeStamp
      << DELIM_RIGHT;
}

/*************************************
 METHOD: Deserialise

 DESC: Deserialises a byte stream into the RollbackTag class

 ARGS: istream& - the stream to deserialise from
 RETURN: void
 **************************************/
void RollbackTag::Deserialise(std::istream& istr) {
  istr.ignore(std::numeric_limits<std::streamsize>::max(), DELIM_LEFT);
  istr >> originalSsv;
  istr.ignore(std::numeric_limits<std::streamsize>::max(), DELIM_VAR_SEPARATOR);
  istr >> time;
  istr.ignore(std::numeric_limits<std::streamsize>::max(), DELIM_VAR_SEPARATOR);
  istr >> type;
  istr.ignore(std::numeric_limits<std::streamsize>::max(), DELIM_VAR_SEPARATOR);
  istr >> realTimeStamp;
  istr.ignore(std::numeric_limits<std::streamsize>::max(), DELIM_RIGHT);
}
