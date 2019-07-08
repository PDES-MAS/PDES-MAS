#ifndef _WRITEPERIOD_H_ 
#define _WRITEPERIOD_H_

using namespace std;
#include "Serialisable.h"
#include "ObjectMgr.h"
#include "SerialisableMultiMap.h"
#include "Log.h"
#include "RollbackList.h"

namespace pdesmas {

  class WritePeriod: public Serialisable {
    private:
      unsigned long fStartTime;
      unsigned long fEndTime;
      AbstractValue* fValue;
      LpId fAgent;
      SerialisableMultiMap<LpId, unsigned long> fAgentReadMap;
    public:
      WritePeriod();
      WritePeriod(const AbstractValue* pValue, unsigned long pStartTime, const LpId& pAgent);
      WritePeriod(const WritePeriod&);
      ~WritePeriod();

      AbstractValue* Read(const LpId&, unsigned long);
      void RemoveReadsBefore(unsigned long);
      void RemoveReadsAfterInclusive(unsigned long, RollbackList&);
      void RemoveReadsByAgent(const LpId&, unsigned long);
      void RemoveAllReads(unsigned long, RollbackList&);

      void SetStartTime(unsigned long);
      unsigned long GetStartTime() const;
      void SetAgent(const LpId&);
      const LpId& GetAgent() const ;
      void SetEndTime(unsigned long);
      unsigned long GetEndTime() const;
      void SetValue(const AbstractValue*);
      AbstractValue* GetValueCopy() const ;

      void Serialise(ostream&) const;
      void Deserialise(istream&);
  };
}
#endif
