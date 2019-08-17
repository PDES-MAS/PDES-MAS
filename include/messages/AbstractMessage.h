  /**
   * \brief Abstract base class which all messages inherit from
   *
   *AbstractMessage.h
   *
   *Author: Michael Lees (mhl)
   *Date: 03/02/2005
   *
   *Description: This is the Abstract Base Class (ABC) from which all
   *other message classes inherit. It defines some standard members
   *which all messages contain. It also specifies two purely virtual
   *methods send and receive which are defined in the three child
   *classes. Each message sub class must also implement the CompareTo
   *method.
   *Revisions: (txo) 11/07/2005 - added de/serialisable methods,
   *comments to compare methods
   */
#ifndef ABSTRACTMESSAGE_H_
#define ABSTRACTMESSAGE_H_

#include "Serialisable.h"
#include "HasOrigin.h"
#include "HasDestination.h"
#include "Types.h"

using namespace std;

namespace pdesmas {


  class Lp;

  class AbstractMessage: public Serialisable,
      public HasOrigin,
      public HasDestination {
    protected:
      /*
       * Register the class in the AbstractPool with a
       * create instance function pointer
       */
      static void RegisterClass(pdesmasType, AbstractMessage* (*)());
      /*
       * Recreate an instance of the class from the AbstractPool using
       * the create instance function pointer
       */
      static AbstractMessage* RecreateInstance(pdesmasType);

    public:
      AbstractMessage();
      virtual ~AbstractMessage();

      /*
       * Virtual get type method for getting the leaf type of the message.
       */
      virtual pdesmasType GetType() const=0;
      /*
       * Virtual Send method implemented in ControlMessage, LoadBalancingMessage,
       * and SimulationMessage to use the three different send queues.
       */
      virtual void SendToLp(Lp *) const=0;
      /*
       * Virtual Receive method implemented in ControlMessage, LoadBalancingMessage,
       * and SimulationMessage to use the three different receive queues.
       */
      virtual void ReceiveToLp(Lp *) const=0;
      /*
       * Virtual Serialise method for serialising the message
       */
      virtual void Serialise(ostream&) const=0;
      /*
       * Virtual Deserialise method for deserialising the message
       */
      virtual void Deserialise(istream&)=0;
  };
}
#endif
