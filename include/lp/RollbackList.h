#ifndef _ROLLBACKLIST_H_
#define _ROLLBACKLIST_H_

using namespace std;

#include "LpId.h"
#include "RollbackTag.h"
namespace pdesmas{
class Lp;
  class RollbackList {
  private:
    map<LpId, unsigned long> list;

  public:
    RollbackList(){}
    ~RollbackList(){}
    int GetSize() const {return list.size();}
    void AddLp(const LpId&, unsigned long);
    void SendRollbacks(Lp*, const RollbackTag&);
    void SendRollbacksAfterStateMigration(Lp*, const RollbackTag&);
  };
}
#endif
