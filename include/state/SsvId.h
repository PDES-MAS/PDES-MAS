#ifndef _SSVID_H_
#define _SSVID_H_

#include "Serialisable.h"

using namespace std;

namespace pdesmas {
  class SsvId : public Serialisable {
  private:
    unsigned long id_;
  public:
    unsigned long id() const;

    unsigned long owner() const;

    bool is_public() const;

  private:
    unsigned long owner_=0;
    bool is_public_=true;
  public:
    SsvId();

    explicit SsvId(unsigned long);

    ~SsvId();


    bool operator==(SsvId const &) const;

    SsvId &operator=(SsvId const &);

    bool operator<(SsvId const &) const;

    void Serialise(ostream &) const override;

    void Deserialise(istream &) override;
  };
}

#endif

