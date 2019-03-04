/*******************************************************************************
 * TinySerializer Library
 * Institution: University of Birmingham
 * A simple global class used for the registering of all serializable Objects 
 * All Serializable Objects may be registered using this class. However abstract
 * serializable objects must be registered with this class. 
 *
 * Last Revision Date: 31/01/2005
 ********************************************************************************/

#ifndef _OBJECTMGR_H_
#define _OBJECTMGR_H_

#include <string>
#include "AbstractMessage.h"
#include "AbstractValue.h"
#include "AbstractPool.h"

using namespace std;

namespace pdesmas {

  //The Abstract Pool map <1, 2> actually contains a map from 2 to 1
  typedef AbstractPool<AbstractMessage*, pdesmasType> MessageClassMap;
  typedef AbstractPool<AbstractValue*, pdesmasType> ValueClassMap;

  extern MessageClassMap* messageClassMap;

  extern ValueClassMap* valueClassMap;

  extern string GetValueString(string msgString);

}
#endif // OBJECTMGR_H
