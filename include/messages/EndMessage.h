  /*!
   \brief End message for an ALP

   This is the EndMessage class used to indicate an agent
   has finished sending it's messages. This message has special routing,
   whereby it is routed through all ports except the one it came from.
   This is used by the shutdown procedure.

   The shutdown procedure uses GVT for the termination condition, the
   simulation is given a set end_time and when GVT=end_time the
   simulation can be safely terminated. Here, as in the previous approach
   each LP sends a termination message with a time-stamp = end_time of
   the simulation(NOTE end_time is usually set to time of last event plus
   one). This message must be broadcasxt to all CLPs (Not ALPs) in the
   system so as to update their LVT. After sending the termination
   message the ALP instigates a GVT calculation to determine the new GVT
   value. When the first agent does this GVT != end_time, although the
   LVT of the terminating agent = end_time and the LVT of all the CLPs =
   end_time, the LVT of all the other ALPs != end_time as none have sent
   their termination message.

   Using GVT in this way resolves the problem of transient messages as
   the GVT algorithm accounts for this.

   When all ALPs have sent their termination message the root CLP must
   periodically instigate gvt calculation. Once all transient messages
   have arrived and GVT is calculated as end_time it is safe to
   terminate.*/
#ifndef ENDMESSAGE_H_
#define ENDMESSAGE_H_

#include "ControlMessage.h"
#include "HasSenderAlp.h"

namespace pdesmas {
  class EndMessage: public ControlMessage, public HasSenderAlp {
    private:
      static AbstractMessage* CreateInstance();

    public:
      EndMessage();
      virtual ~EndMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
