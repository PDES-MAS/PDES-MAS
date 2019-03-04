/*
 * Log.cpp
 *
 *  Created on: 31 Jan 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include "Log.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
using namespace pdesmas;

Log::~Log() {
    os << std::endl;
    fprintf(stderr, "%s", os.str().c_str());
    fflush(stderr);
}

std::ostringstream& Log::Get(TLogLevel level) {
  os << "- " << ToString(level) << " " << NowTime() << ": ";
  messageLevel = level;
  return os;
}

std::string Log::ToString(TLogLevel pLevel) {
  switch (pLevel) {
    case logERROR:
      return "ERROR";
    case logWARNING:
      return "WARNING";
    case logINFO:
      return "INFO";
    case logFINE:
      return "FINE";
    case logFINER:
      return "FINER";
    case logFINEST:
      return "FINEST";
    default:
      return "ERROR";
  }
}

char* Log::NowTime() {
  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char* tempCharArray = asctime(timeinfo);
  tempCharArray[strlen(tempCharArray) -1] = '\0';
  return tempCharArray;
}
