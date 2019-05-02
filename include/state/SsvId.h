#ifndef _SSVID_H_
#define _SSVID_H_

#include "Serialisable.h"

using namespace std;

namespace pdesmas {
  class SsvId : public Serialisable {
  private:
    int variable_id_;

  public:
    SsvId();

    explicit SsvId(int);

    ~SsvId();

    bool operator==(SsvId const &) const;

    SsvId &operator=(SsvId const &);

    bool operator<(SsvId const &) const;

    void Serialise(ostream &) const override;

    void Deserialise(istream &) override;
  };
}

#endif

