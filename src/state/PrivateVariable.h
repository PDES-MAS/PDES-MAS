//
// Created by pill on 19-5-9.
//

#ifndef PDES_MAS_PRIVATEVARIABLE_H
#define PDES_MAS_PRIVATEVARIABLE_H

#include <variant>
#include <string>
#include <types/Point.h>

using namespace std;
namespace pdesmas {
  template<class T>
  class PrivateVariable {
  private:
    T value_;
  public:
    void set_value(T v);

    T get_value();
  };


}


#endif //PDES_MAS_PRIVATEVARIABLE_H
