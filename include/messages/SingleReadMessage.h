  /*!
   \brief Class for reading a single SSV

   An instance of SingleReadMessage defines a message for an ALP
   ALP reading the value of an individual Shared State Variable.
   */
#ifndef SINGLEREADMESSAGE_H_
#define SINGLEREADMESSAGE_H_

#include "SharedStateMessage.h"
#include "HasSSVID.h"

namespace pdesmas {
  class SingleReadMessage: public SharedStateMessage,
    public HasSSVID {
    private:
      static AbstractMessage* CreateInstance();

    public:
      SingleReadMessage();
      virtual ~SingleReadMessage();

      pdesmasType GetType() const;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
