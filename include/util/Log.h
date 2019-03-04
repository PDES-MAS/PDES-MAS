/*
 * Log.h
 *
 *  Created on: 31 Jan 2011
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef LOG_H_
#define LOG_H_

#include <sstream>

namespace pdesmas {
  enum TLogLevel {
    logERROR, logWARNING, logINFO, logFINE, logFINER, logFINEST
  };
  class Log {
    public:
      Log(){};
      virtual ~Log();
      std::ostringstream& Get(TLogLevel level = logINFO);
    public:
      static TLogLevel ReportingLevel() {return logFINE;};
    protected:
      std::ostringstream os;
    private:
      Log(const Log&);
      Log& operator =(const Log&);
      std::string ToString(TLogLevel);
      char* NowTime();
    private:
      TLogLevel messageLevel;
  };
#define LOG(level) \
if (level > Log::ReportingLevel()) ; \
else Log().Get(level)
}
#endif /* LOG_H_ */
