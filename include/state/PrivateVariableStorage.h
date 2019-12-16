//
// Created by pill on 19-5-2.
//

#ifndef PDES_MAS_PRIVATEVARIABLESTORAGE_H
#define PDES_MAS_PRIVATEVARIABLESTORAGE_H


#include <map>
#include <AbstractValue.h>
#include <types/SerialisableList.h>
#include "WritePeriod.h"

using namespace std;
namespace pdesmas {
  class PrivateVariableStorage {
  private:
    map<unsigned long,SerialisableList<WritePeriod> > variable_id_wp_map_;
  public:
    void AddVariable(const SsvId& variable_id, AbstractValue *v, unsigned long timestamp);

    AbstractValue *ReadVariable(const SsvId& variable_id, unsigned long timestamp);

    void WriteVariable(const SsvId& variable_id, AbstractValue *value, unsigned long timestamp);

    void PerformRollback(unsigned long timestamp);

    void CleanUp(unsigned long timestamp);
  };
}


#endif //PDES_MAS_PRIVATEVARIABLESTORAGE_H
