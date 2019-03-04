#ifndef _ROLLBACKTAG_H_
#define _ROLLBACKTAG_H_

using namespace std;

#include "Serialisable.h"
#include "SsvId.h"
using namespace pdesmas;
 

namespace pdesmas{


/*!

\brief Used to uniquely identify rollbacks with particular straggler writes

RollbackTag.h

Author: Michael Lees (mhl) 
Date: (18/01/05)


Description: Used in synchronisation to ensure an LP isn't rolled back
twice for the same rollback. Identifies the rollback uniquely by the
state variable it originated on and the virtual time at which it
occurred.

Revisions:

*/

class RollbackTag : public pdesmas::Serialisable
{

 private:
  //Slots
  SsvId originalSsv; 
  /**<The original shared state variable where rb originated */
  unsigned long time;
  /**< The original virtual time of the straggler write */
  int type;
  
  unsigned long int realTimeStamp; /**< The real time stamp of the rbTag, this is
			   the real time when the first rollback tag
			   is generated due to the straggler write */
 public:
  RollbackTag(){}
  RollbackTag(SsvId const&, unsigned long, int);
  ~RollbackTag(){}

  //Member functions
  bool operator==(RollbackTag const& rbTag) const; 
  /**< Comparison operator to check if two rollback tags are
     equal. Equality requires all member values to be the same */
  bool operator<(RollbackTag const& rbTag) const;
  /**< Less than operator, defined in terms of virtual time. */
  void operator=(RollbackTag const& rbTag);
  /**< Assignment operator */

  SsvId GetSsv() const {return originalSsv;}
  /**< Returns the Id of the shared state variable associated with the tag */
  void SetSsv(SsvId const& ssv){originalSsv = ssv;}
  /**< Sets the Id of the shared state variable associated with the tag */
  
  unsigned long GetTime() const {return time;}
  /**< Returns the virtual time of the rollback tag */
  void SetTime(unsigned long t){time=t;};
  /**< Sets the virtual time of the rollback tag */
  int GetType() const {return type;};
  void Serialise(std::ostream &) const;
  /**< Method used to serialise the rollback tag when it is sent as
     part of a RollbackMessage */
  void Deserialise(std::istream &);
  /**< Method which deserialises the rollback tag after sending */

  
};

}
#endif
