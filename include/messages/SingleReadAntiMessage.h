#ifndef SINGLEREADANTIMESSAGE_H_
#define SINGLEREADANTIMESSAGE_H_

#include "AntiMessage.h"
#include "HasSSVID.h"

namespace pdesmas {
  class SingleReadAntiMessage: public AntiMessage, public HasSSVID {
    private:
      static AbstractMessage* CreateInstance();

    public:
      SingleReadAntiMessage();
      virtual ~SingleReadAntiMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
