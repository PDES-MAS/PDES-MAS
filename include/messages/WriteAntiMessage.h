#ifndef WRITEANTIMESSAGE_H_
#define WRITEANTIMESSAGE_H_

#include "AntiMessage.h"
#include "HasSSVID.h"

namespace pdesmas {
  class WriteAntiMessage: public AntiMessage, public HasSSVID {
    private:
      static AbstractMessage* CreateInstance();

    public:
      WriteAntiMessage();
      virtual ~WriteAntiMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
