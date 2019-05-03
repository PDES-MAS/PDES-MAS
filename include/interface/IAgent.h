/*
 * IAgent.h
 *
 *  Created on: 9 Sep 2010
 *      Author: Dr B.G.W. Craenen (b.craenen@cs.bham.ac.uk)
 */

#ifndef IAGENT_H_
#define IAGENT_H_

#include "Alp.h"
#include "IdentifierHandler.h"
#include "Thread.h"
#include "HasIDLVTMap.h"
#include "HasMPIInfo.h"
#include "HasResponseMessage.h"
#include "FilePrint.h"
#include "EndMessage.h"

namespace pdesmas {
  class IAgent : public HasResponseMessage {
  private:
    Alp *fIAlp;
    IdentifierHandler *fIdentifierHandler;
    Semaphore fResponseSemaphore;
    Semaphore fPostReceiveSemaphore;
    Semaphore fPostOutsideSemaphore;
    Mutex fInsideMutex;
    bool fHasResponseMessageWaiting;
    bool fHasOutsideMessageWaiting;

    FilePrint fFilePrint;

  public:
    IAgent(unsigned int, unsigned int, unsigned int, unsigned int, unsigned long const &, unsigned long const &,
           const Initialisor &, Alp *);

    void SetIgnoreID();

    unsigned long GetGvt() const;

    AbstractMessage *Read(long, int, unsigned long);

    AbstractMessage *WriteInt(long, int, int, unsigned long);

    AbstractMessage *WriteDouble(long, int, double, unsigned long);

    AbstractMessage *WritePoint(long, int, const Point, unsigned long);

    AbstractMessage *WriteString(long, int, const string, unsigned long);

    AbstractMessage *RangeQuery(long, unsigned long, const Point, const Point);

    void SendGVTMessage();

    void SendEndMessage();

    AbstractMessage *WaitForMessage();

    AbstractMessage *GetOutsideMessage() const;

    void SignalReceiveProcess();

    void SignalOutsideRollback();

    void SignalPostReceive();

    void SignalPostOutside();

    bool SignalResponse(unsigned long, long);

    void SignalResponse();

    void PostReceiveWait();

    void PostOutsideWait();

    bool HasResponseMessageWaiting() const;

    bool HasOutsideMessageWaiting() const;

    void SetResponseMessageWaiting(bool);

    void SetOutsideMessageWaiting(bool);

    void Lock();

    void Unlock();

    void *MyThread(void *);
  };
}

#endif /* IAGENT_H_ */
