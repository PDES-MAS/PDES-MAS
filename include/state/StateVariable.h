#ifndef _STATEVARIABLE_H_
#define _STATEVARIABLE_H_

#include "WritePeriod.h"
#include "RollbackList.h"
#include "WriteStatus.h"
#include "SerialisableList.h"
#include "Log.h"

using namespace std;

namespace pdesmas {
  class StateVariable: public Serialisable {
    private:
      SsvId fStateVariableID;
      SerialisableList<WritePeriod> fWritePeriodList;

    public:
      StateVariable();
      StateVariable(const SsvId&);
      StateVariable(const StateVariable&);
      ~StateVariable();

      const SsvId& GetVariableId() const;
      void AddWritePeriod(const AbstractValue*, unsigned long, const LpId&);
      void RemoveWritePeriods(unsigned long);
      const SerialisableList<WritePeriod>& GetWritePeriodList() const;

      AbstractValue* Read(const LpId&, unsigned long);
      pair<unsigned long, AbstractValue*> ReadWithoutRecord(unsigned long) const;
      void WriteWithRollback(const LpId&, const AbstractValue*, unsigned long, WriteStatus&, RollbackList&);
      void PerformReadRollback(const LpId&, unsigned long);
      void PerformWriteRollback(const LpId&, unsigned long, RollbackList&);
      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
