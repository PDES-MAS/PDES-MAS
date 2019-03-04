/*
 * Types.h
 *
 *  Created on: 24 Oct 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#ifndef TYPES_H_
#define TYPES_H_

#include "Helper.h"

using namespace std;

namespace pdesmas {
  enum pdesmasType {
    VALUEINT = 0,
    VALUEDOUBLE = 1,
    VALUEPOINT = 2,
    VALUESTRING = 3,
    VALUELONG = 4,
    ENDMESSAGE = 5,
    GVTCONTROLMESSAGE = 6,
    GVTREQUESTMESSAGE = 7,
    GVTVALUEMESSAGE = 8,
    RANGEQUERYANTIMESSAGE = 9,
    RANGEQUERYMESSAGE = 10,
    RANGEUPDATEMESSAGE = 11,
    ROLLBACKMESSAGE = 12,
    SINGLEREADANTIMESSAGE = 13,
    SINGLEREADMESSAGE = 14,
    SINGLEREADRESPONSEMESSAGE = 15,
    STATEMIGRATIONMESSAGE = 16,
    WRITEANTIMESSAGE = 17,
    WRITEMESSAGE = 18,
    WRITERESPONSEMESSAGE = 19
  };

  inline istream& operator>>(istream& pIstream, pdesmasType& pType) {
    int type;
    pIstream >> type;
    switch(type) {
      case VALUEINT : pType = VALUEINT; break;
      case VALUEDOUBLE : pType = VALUEDOUBLE; break;
      case VALUEPOINT : pType = VALUEPOINT; break;
      case VALUESTRING : pType = VALUESTRING; break;
      case VALUELONG : pType = VALUELONG; break;
      case ENDMESSAGE : pType = ENDMESSAGE; break;
      case GVTCONTROLMESSAGE : pType = GVTCONTROLMESSAGE; break;
      case GVTREQUESTMESSAGE : pType = GVTREQUESTMESSAGE; break;
      case GVTVALUEMESSAGE : pType = GVTVALUEMESSAGE; break;
      case RANGEQUERYANTIMESSAGE : pType = RANGEQUERYANTIMESSAGE; break;
      case RANGEQUERYMESSAGE : pType = RANGEQUERYMESSAGE; break;
      case RANGEUPDATEMESSAGE : pType = RANGEUPDATEMESSAGE; break;
      case ROLLBACKMESSAGE : pType = ROLLBACKMESSAGE; break;
      case SINGLEREADANTIMESSAGE : pType = SINGLEREADANTIMESSAGE; break;
      case SINGLEREADMESSAGE : pType = SINGLEREADMESSAGE; break;
      case SINGLEREADRESPONSEMESSAGE : pType = SINGLEREADRESPONSEMESSAGE; break;
      case STATEMIGRATIONMESSAGE : pType = STATEMIGRATIONMESSAGE; break;
      case WRITEANTIMESSAGE : pType = WRITEANTIMESSAGE; break;
      case WRITEMESSAGE : pType = WRITEMESSAGE; break;
      case WRITERESPONSEMESSAGE : pType = WRITERESPONSEMESSAGE; break;
    }
    return pIstream;
  }

  extern pdesmasType GetTypeID(string pString);

  bool IsSimulationMessage(pdesmasType);
  bool IsSharedStateMessage(pdesmasType);
}

#endif /* TYPES_H_ */
