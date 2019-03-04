/*
 * Types.cpp
 *
 *  Created on: 26 Oct 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include "Types.h"
#include "Serialisable.h"

namespace pdesmas {

  extern pdesmasType GetTypeID(string pString) {
    size_t start = pString.find_first_not_of(DELIM_LEFT);
    size_t end = 0;
    if (start != string::npos) end = pString.find_first_of(DELIM_VAR_SEPARATOR,
        start + 1);
    if (end == string::npos) end = pString.length();
    return Helper::stream_cast<pdesmasType, string>(
        pString.substr(start, end - start));
  }

  bool IsSimulationMessage(pdesmasType pType) {
    switch (pType) {
      case SINGLEREADMESSAGE:
        return true;
      case SINGLEREADRESPONSEMESSAGE:
        return true;
      case SINGLEREADANTIMESSAGE:
        return true;
      case WRITEMESSAGE:
        return true;
      case WRITERESPONSEMESSAGE:
        return true;
      case WRITEANTIMESSAGE:
        return true;
      case RANGEQUERYMESSAGE:
        return true;
      case RANGEQUERYANTIMESSAGE:
        return true;
      case ROLLBACKMESSAGE:
        return true;
      default:
        return false;
    }
  }

  bool IsSharedStateMessage(pdesmasType pType) {
    switch (pType) {
      case SINGLEREADMESSAGE:
        return true;
      case WRITEMESSAGE:
        return true;
      case RANGEQUERYMESSAGE:
        return true;
      default:
        return false;
    }
  }
}
