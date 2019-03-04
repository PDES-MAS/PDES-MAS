  /*!
   \brief Class for updating a single SSV

   An instance of SingleReadMessage defines a message for an ALP
   updating the value of an individual Shared State Variable. A successful update
   will create a new write period at the destination CLP
   */
#ifndef WRITEMESSAGE_H_
#define WRITEMESSAGE_H_

#include "SharedStateMessage.h"
#include "HasSSVID.h"
#include "HasValue.h"

namespace pdesmas {
  class WriteMessage: public SharedStateMessage,
      public HasSSVID,
      public HasValue {
    private:
      static AbstractMessage* CreateInstance();

    public:
      WriteMessage();
      virtual ~WriteMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
