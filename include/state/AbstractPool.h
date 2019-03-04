#ifndef ABSTRACTPOOL_H
#define ABSTRACTPOOL_H

/********************************************************************************
 * AbstractValue.h 
 * 
 * Institution: University of Birmingham
 * Author: Ton Oguara (txo@cs.bham.ac.uk)
 *
 * Description:
 * This is a top level singleton class for all Serialisable abstract classes  
 * All serializable objects may be registed with this class
 *  
 * Last Revision Date: 25/02/2005 
 * mhl - 26/04/05 - changed line 127 changed e to excep, e is used 
 * as constant in probabilistic blocking
 ********************************************************************************/

#include <iostream>
#include <map>

#include <cstdlib>
#include <exception>

using namespace std;

namespace pdesmas {

  template<class AbstractObject, typename AbsObjType,
      typename ObjectMaker = AbstractObject(*)()>
  class AbstractPool {
    private:
      static AbstractPool* sInstance; //the only instance
      typedef typename std::map<AbsObjType, ObjectMaker> ObjectInstance; //maintain object instances
      ObjectInstance mappings;

    protected:
      // making this class a singleton
      AbstractPool();//prevents clients creating a new instance
      AbstractPool(const AbstractPool&) {
      } // prevents clients from creating a copy
      AbstractPool& operator=(const AbstractPool&) {
      } //prevent client assingnment implementation

    public:
      // the Instance member function ;
      static AbstractPool* Instance() //create an instance of this class
      {
        if (!sInstance) sInstance = new AbstractPool;
        return sInstance;
      }

      bool RegisterObject(const AbsObjType&, ObjectMaker); // register the objects
      bool UnregisterObject(const AbsObjType&); // unregister the objects
      AbstractObject CreateObject(const AbsObjType&); // recreate an object
  };

  /* the implementation */
  /*==============================================================================
   * Initialize the static variable
   *-----------------------------------------------------------------------------*/
  template<class AbstractObject, typename AbsObjType, typename ObjectMaker>
  AbstractPool<AbstractObject, AbsObjType, ObjectMaker>* AbstractPool<
      AbstractObject, AbsObjType, ObjectMaker>::sInstance = 0; //initialize pointer

  /*==============================================================================
   * Default Constructor - Nothing to do for now
   *-----------------------------------------------------------------------------*/
  template<class AbstractObject, typename AbsObjType, typename ObjectMaker>
  AbstractPool<AbstractObject, AbsObjType, ObjectMaker>::AbstractPool() {
    // nothing for now
  }

  /*==============================================================================
   * Register ClassID and their Object instances in the Map container
   *
   * Parameters:
   * 			absID: contified reference variable indicating the ObjectID
   * 			makeObj: The Object instance
   *-----------------------------------------------------------------------------*/

  template<class AbstractObject, typename AbsObjType, typename ObjectMaker>
  bool AbstractPool<AbstractObject, AbsObjType, ObjectMaker>::RegisterObject(
      const AbsObjType& absID, ObjectMaker makeObj) {

    typename ObjectInstance::const_iterator cpos = mappings.find(absID);
    if (cpos == mappings.end()) return mappings.insert(
        typename std::map<AbsObjType, ObjectMaker>::value_type(absID, makeObj)).second;

    return (false);
  }

  /*==============================================================================
   * Remove a classID and its Object instance from the Map Container
   *
   * Parameters:
   * 			absID: contified reference variable indicating the ObjectID
   *-----------------------------------------------------------------------------*/
  template<class AbstractObject, typename AbsObjType, typename ObjectMaker>
  bool AbstractPool<AbstractObject, AbsObjType, ObjectMaker>::UnregisterObject(
      const AbsObjType& absID) {
    return mappings.erase(absID) == 1;
  }

  /*==============================================================================
   * Recreate Object Instances. Will be modified to throw an exception if the
   * Object instance can not be created.
   *
   * Parameters:
   * 			absID: contified reference variable indicating the ObjectID
   *-----------------------------------------------------------------------------*/
  template<class AbstractObject, typename AbsObjType, typename ObjectMaker>
  AbstractObject AbstractPool<AbstractObject, AbsObjType, ObjectMaker>::CreateObject(
      const AbsObjType& absID) {
    typename ObjectInstance::const_iterator cpos = mappings.find(absID);
    if (cpos != mappings.end()) {
      return (cpos->second)();
    } else {
      AbstractObject* aa = new AbstractObject;

      //Object is not registered and thus can't be deserialized. Should throw an exception here
      std::cerr
          << "AbstractPool::CreateObject# Trying to create unknown object Type: "
          << absID << "\n";
      return (*aa); //return 0; the Object is not found
    }
  }
}
#endif // ABSTRACTPOOL_H
