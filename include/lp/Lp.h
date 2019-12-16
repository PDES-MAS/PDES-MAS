/*!

 \brief Base class from which Lps inherit

 Lp.h

 Author: Michael Lees (mhl)
 Date: 15/04/2005

 This class implements the base class for all logical process. At
 present the system implementes two sub classes of LP. The Clp and the
 Alp. The Lp class provides the functionallity common between these two
 Lps.

 Revisions:

 */

#ifndef _LP_H_
#define _LP_H_

#include "AbstractMessage.h"
#include "MessageQueue.h"
#include "GvtCalculator.h"
#include "LpId.h"
#include "MpiInterface.h"
#include <deque>
#include "Router.h"
#include "Mutex.h"
#include "HasMPIInfo.h"

using namespace std;

namespace pdesmas {
  class Lp : public HasMPIInfo {
    private:
      Mutex fProcessMutex;
    protected:
      // The current GVT of this Lp
      unsigned long fGVT;
      // Start time of my Simulation
      unsigned long fStartTime;
      // End time of my Simulation
      unsigned long fEndTime;
      // Array indicating from which alps endmessage received, indexed by MPI_Rank
      bool* fEndMessagesReceived;
      // Class responsible for processing incoming GVT messages and performing GVT calculations
      GvtCalculator* fGVTCalculator;

      void PreProcessSendMessage(SimulationMessage*);
      void PreProcessReceiveMessage(const SimulationMessage*);

    public:
      // Normal receive queue for simulation and control messages
      MessageQueue* fReceiveMessageQueue;
      // Send queue for simulation messages. Messages in this queue are blocked by the window if a windowing mechanism is used
      MessageQueue* fSendMessageQueue;
      // For control messages - Non blocking even with windows
      MessageQueue* fSendControlMessageQueue;
      // A Clp has two additional queues used for load balancing and state migration - These queues were moved here from Clp.h - by Design
      MessageQueue* fReceiveLoadBalancingMessageQueue;
      // Send queue for load balancing messages
      MessageQueue* fSendLoadBalancingMessageQueue;
      // Class which provides the interface from the Lp to MPI. If a different message passing system is required this class should be redefined (eg., PVM_Interface)
      MpiInterface* fMPIInterface;

      // Default constructor
      Lp();
      // Default deconstructor
      ~Lp();
      /* Virtual method indicating how messages are transferred from the
       sendQ's of the Lp to the MpiInterface. This method is purely
       virtual for Lp and must be redefined for any subclass */
      virtual void Send()=0;
      /* Virtual method indicating how messages are taken from recvQ and
       passed to ProcessMessage method. This has no definition in Lp, so
       it must be redefined for sub classes. */
      virtual void Receive()=0;
      /* This is the method which describes the main loop of the
       Lp. This method has a definition in Lp but can be redefined for
       sub classes. By default the method loops while the simulation is
       running and waits for incoming recv signals (using
       RecvSemaphore). When signalled the method processes incoming
       messages using the ProcessMessage method. */
      void Run();
      // True if the Lp has received end messages from all other Lps
      bool AllEndMessagesReceived() const;
      // Returns the current LVT of the Lp
      virtual unsigned long GetLvt() const=0;
      // Returns Gvt of the Lp
      unsigned long GetGvt() const;
      // Sets Gvt of the Lp
      virtual void SetGvt(unsigned long)=0;
      // Returns the end time of this Lps Simulation
      unsigned long GetEndTime() const;
      // Initialisation hook - nothing by default
      virtual void Initialise()=0;
      // Shut down hook - nothing by default
      virtual void Finalise()=0;
      // We can terminate when our gvt is past endTime
      // Returns true when we have reached the termination condition, ie., gvt >= endTime
      virtual bool TerminationCondition() const;
      void Lock();
      void Unlock();
      // Method used when messages are placed in the send queue. This signals the semaphore in the send thread to initiate an MPI send
      void SignalSend();
  };
}
#endif
