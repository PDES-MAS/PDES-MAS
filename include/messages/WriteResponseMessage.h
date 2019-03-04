#ifndef WRITERESPONSEMESSAGE_H_
#define WRITERESPONSEMESSAGE_H_

#include "ResponseMessage.h"
#include "HasWriteStatus.h"
#include "WriteMessage.h"

namespace pdesmas {
  class WriteResponseMessage: public ResponseMessage,
      public HasWriteStatus {
    private:
      static AbstractMessage* CreateInstance();

    public:
      WriteResponseMessage();
      virtual ~WriteResponseMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
