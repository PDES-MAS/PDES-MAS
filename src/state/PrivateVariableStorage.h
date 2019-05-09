//
// Created by pill on 19-5-2.
//

#ifndef PDES_MAS_PRIVATEVARIABLESTORAGE_H
#define PDES_MAS_PRIVATEVARIABLESTORAGE_H


#include "PrivateVariable.h"
#include <map>

using namespace std;
namespace pdesmas {
  class PrivateVariableStorage {
    map<unsigned long, PrivateVariable *> private_variables_;
  };
}


#endif //PDES_MAS_PRIVATEVARIABLESTORAGE_H
