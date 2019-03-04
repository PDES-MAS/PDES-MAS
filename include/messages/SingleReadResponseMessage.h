#ifndef SINGLEREADRESPONSEMESSAGE_H_
#define SINGLEREADRESPONSEMESSAGE_H_

#include "ResponseMessage.h"
#include "HasValue.h"
#include "SingleReadMessage.h"

namespace pdesmas {
  class SingleReadResponseMessage: public ResponseMessage,
      public HasValue {
    private:
      static AbstractMessage* CreateInstance();

    public:
      SingleReadResponseMessage();
      virtual ~SingleReadResponseMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
