#ifndef STATEMIGRATIONMESSAGE_H_
#define STATEMIGRATIONMESSAGE_H_

#include "LoadBalancingMessage.h"
#include "HasStateVariableMap.h"

namespace pdesmas {
  class StateMigrationMessage: public LoadBalancingMessage,
      public HasStateVariableMap {
    private:
      static AbstractMessage* CreateInstance();

    public:
      StateMigrationMessage();
      virtual ~StateMigrationMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
