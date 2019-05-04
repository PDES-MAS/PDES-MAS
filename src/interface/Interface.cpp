/*
 * Interface.cpp
 *
 *  Created on: 18 Jun 2010
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */
#include <jni.h>
#include <string>
#include "Interface.h"
#include "assert.h"
#include "Agent.h"
#include "Helper.h"
#include "Log.h"

using namespace std;
using namespace pdesmas;

JNIEXPORT void JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_construct(JNIEnv * env, jobject methodRef, jlong pNumberOfAlps, jlong pNumberOfClps, jlong pStartTime, jlong pEndTime) {
	  fNumberOfAlps = (int) pNumberOfAlps;
	  fNumberOfClps = (int) pNumberOfClps;
	  fStartTime = (int) pStartTime;
	  fEndTime = (int) pEndTime;
	  int providedThreadSupport;
	  MPI_Init_thread(NULL, NULL, MPI_THREAD_SERIALIZED, &providedThreadSupport);
	  assert(providedThreadSupport==MPI_THREAD_SERIALIZED);
	  MPI_Comm_size(MPI_COMM_WORLD, &fCommSize);
	  MPI_Comm_rank(MPI_COMM_WORLD, &fCommRank);
}

JNIEXPORT void JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_initialise(JNIEnv * env, jobject methodRef, jstring pDataLocation) {
  MPI_Barrier(MPI_COMM_WORLD);
  const char* dataLocation;
  dataLocation = env->GetStringUTFChars(pDataLocation, NULL);
  if (dataLocation == NULL) {
	  return; /* OutOfMemoryError already thrown */
  }
  string traceDir (dataLocation);
  env->ReleaseStringUTFChars(pDataLocation, dataLocation);
  if (fCommRank < fNumberOfClps) {
    fClp = new Clp(fCommRank, fCommSize, fNumberOfClps, fNumberOfAlps, fStartTime, fEndTime, traceDir);
    fClp->Run();
  } else {
    fIAgent = new Agent(fCommRank, fCommSize, fNumberOfClps, fNumberOfAlps, fStartTime, fEndTime, traceDir);
  }
}

JNIEXPORT void JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_finalise(JNIEnv* env, jobject methodRef) {
  if (fCommRank >= fNumberOfClps) {
    fIAgent->SendEndMessage();
  }
  MPI_Finalize();
}

JNIEXPORT jint JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_getRank
  (JNIEnv *, jobject) {
  return fCommRank;
}

JNIEXPORT jint JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_getSize
  (JNIEnv *, jobject) {
	return fCommSize;
}

JNIEXPORT jint JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_getGVT
  (JNIEnv *, jobject) {
	return fIAgent->GetGvt();
}

JNIEXPORT void JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_sendGVTMessage
  (JNIEnv * env, jobject methodRef) {
  if (checkOutsideRollback(env, methodRef, -1)) {
    fIAgent->Unlock();
    return;
  }
  fIAgent->SendGVTMessage();
}

JNIEXPORT jobject JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_read
  (JNIEnv* env, jobject methodRef, jlong pAgentId, jlong pObjectId, jint pVariableId, jint pTime) {
  if (checkOutsideRollback(env, methodRef, pAgentId)) {
    fIAgent->Unlock();
    return NULL;
  }
  LOG(logFINEST) << "PDESMASInterface::read(" << fCommRank << "): " << pAgentId << ", " << pObjectId << ", " << pVariableId << ", " << pTime;
  jobject result = readResponse(env, methodRef, fIAgent->Read((long) pAgentId, (long) pObjectId, (int) pVariableId, pTime), pAgentId);
  fIAgent->SetResponseMessageWaiting(false);
  fIAgent->SignalReceiveProcess();
  fIAgent->Unlock();
  fIAgent->PostReceiveWait();
  return result;
}

JNIEXPORT jboolean JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJIII
  (JNIEnv* env, jobject methodRef, jlong pAgentId, jlong pObjectId, jint pVariableId, jint pValue, jint pTime) {
  if (checkOutsideRollback(env, methodRef, pAgentId)) {
    fIAgent->Unlock();
    return false;
  }
  LOG(logFINEST) << "PDESMASInterface#writeInt(" << fCommRank << "): " << pAgentId << ", " << pObjectId << ", " << pVariableId << ", " << pValue << ", " << pTime;
  jboolean result = writeResponse(env, methodRef, fIAgent->WriteInt((long) pAgentId, (long) pObjectId, (int) pVariableId, pValue, pTime), pAgentId);
  fIAgent->SetResponseMessageWaiting(false);
  fIAgent->SignalReceiveProcess();
  fIAgent->Unlock();
  fIAgent->PostReceiveWait();
  return result;
}

JNIEXPORT jboolean JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJIDI
  (JNIEnv* env, jobject methodRef, jlong pAgentId, jlong pObjectId, jint pVariableId, jdouble pValue, jint pTime) {
  if (checkOutsideRollback(env, methodRef, pAgentId)) {
    fIAgent->Unlock();
    return false;
  }
  LOG(logFINEST) << "PDESMASInterface#writeDouble(" << fCommRank << "): " << pAgentId << ", " << pObjectId << ", " << pVariableId << ", " << pValue << ", " << pTime;
  jboolean result = writeResponse(env, methodRef, fIAgent->WriteDouble((long) pAgentId, (long) pObjectId, (int) pVariableId, pValue, pTime), pAgentId);
  fIAgent->SetResponseMessageWaiting(false);
  fIAgent->SignalReceiveProcess();
  fIAgent->Unlock();
  fIAgent->PostReceiveWait();
  return result;
}

JNIEXPORT jboolean JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJILmwgrid_middleware_distributedobject_Location_2I
  (JNIEnv* env, jobject methodRef, jlong pAgentId, jlong pObjectId, jint pVariableId, jobject pValue, jint pTime) {
  if (checkOutsideRollback(env, methodRef, pAgentId)) {
    fIAgent->Unlock();
    return false;
  }
  Point* valuePoint = getPoint(env, pValue);
  if (valuePoint == NULL) return false;
  LOG(logFINEST) << "PDESMASInterface#writePoint(" << fCommRank << "): " << pAgentId << ", " << pObjectId << ", " << pVariableId << ", " << valuePoint << ", " << pTime;
  jboolean result = writeResponse(env, methodRef, fIAgent->WritePoint((long) pAgentId, (long) pObjectId, pVariableId, *valuePoint, pTime), pAgentId);
  assert(valuePoint != 0);
  delete valuePoint;
  fIAgent->SetResponseMessageWaiting(false);
  fIAgent->SignalReceiveProcess();
  fIAgent->Unlock();
  fIAgent->PostReceiveWait();
  return result;
}

JNIEXPORT jboolean JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJILjava_lang_String_2I
  (JNIEnv* env, jobject methodRef, jlong pAgentId, jlong pObjectId, jint pVariableId, jstring pValue, jint pTime) {
  if (checkOutsideRollback(env, methodRef, pAgentId)) {
    fIAgent->Unlock();
    return false;
  }
  const char* valueCStr;
  valueCStr = env->GetStringUTFChars(pValue, NULL);
  if (valueCStr == NULL) {
	  return false; /* OutOfMemoryError already thrown */
  }
  LOG(logFINEST) << "PDESMASInterface#writeString(" << fCommRank << "): " << pAgentId << ", " << pObjectId << ", " << pVariableId << ", " << valueCStr << ", " << pTime;
  AbstractMessage* responseMessage = fIAgent->WriteString((long) pAgentId, (long) pObjectId, (int) pVariableId, valueCStr, pTime);
  env->ReleaseStringUTFChars(pValue, valueCStr);
  jboolean result = writeResponse(env, methodRef, responseMessage, pAgentId);
  fIAgent->SetResponseMessageWaiting(false);
  fIAgent->SignalReceiveProcess();
  fIAgent->Unlock();
  fIAgent->PostReceiveWait();
  return result;
}

JNIEXPORT jobject JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_rangeQuery
  (JNIEnv* env, jobject methodRef, jlong pAgentId, jint pTime, jobject pStartValue, jobject pEndValue) {
  if (checkOutsideRollback(env, methodRef, pAgentId)) {
    fIAgent->Unlock();
    jboolean value = false;
    jclass booleanClass = env->FindClass("java/lang/Boolean");
    jmethodID methodID = env->GetMethodID(booleanClass, "<init>", "(Z)V");
    jobject booleanObject = env->NewObject(booleanClass, methodID, value);
    return booleanObject;
    //TODO: confirm this is correct
  }
  Point* startValuePoint = getPoint(env, pStartValue);
  if (startValuePoint == NULL) return NULL;
  Point* endValuePoint = getPoint(env, pEndValue);
  if (endValuePoint == NULL) return NULL;
  LOG(logFINEST) << "Interface::rangeQuery(" << fCommRank << "): " << pAgentId << ", " << pTime << ", " << *startValuePoint << ", " << *endValuePoint;
  jobject result = rangeQueryResponse(env, methodRef, fIAgent->RangeQuery((long) pAgentId, pTime, *startValuePoint, *endValuePoint), pAgentId);
  assert(startValuePoint != 0);
  delete startValuePoint;
  assert(endValuePoint != 0);
  delete endValuePoint;
  fIAgent->SetResponseMessageWaiting(false);
  fIAgent->SignalReceiveProcess();
  fIAgent->Unlock();
  fIAgent->PostReceiveWait();
  return result;
}

bool checkOutsideRollback(JNIEnv* env, jobject refObject, jlong agentID) {
  fIAgent->Lock();
  if (!fIAgent->HasOutsideMessageWaiting())
    return false;
  RollbackMessage* rollbackMessage = static_cast<RollbackMessage*>(fIAgent->GetOutsideMessage());
  fIAgent->SetOutsideMessageWaiting(false);
  jlong rollbackAgentID = (jlong) rollbackMessage->GetOriginalAlp().GetId();
  jint rollbackTime = (jint) rollbackMessage->GetTimestamp();
  if (agentID == rollbackAgentID) {
    fIAgent->SetIgnoreID();
    throwRollbackException(env, rollbackAgentID, rollbackTime);
    assert (rollbackMessage != 0);
    delete rollbackMessage;
    fIAgent->SignalOutsideRollback();
    fIAgent->Unlock();
    fIAgent->PostOutsideWait();
    fIAgent->Lock();
    return true;
  }
  rollbackCallback(env, refObject, rollbackAgentID, rollbackTime);
  assert (rollbackMessage != 0);
  delete rollbackMessage;
  fIAgent->SignalOutsideRollback();
  fIAgent->Unlock();
  fIAgent->PostOutsideWait();
  return checkOutsideRollback(env, refObject, agentID);
}

jobject readResponse(JNIEnv* env, jobject refObject, AbstractMessage* responseMessage, jlong agentID) {
  switch (responseMessage->GetType()) {
    case SINGLEREADRESPONSEMESSAGE : {
      SingleReadResponseMessage* response = static_cast<SingleReadResponseMessage*>(responseMessage);
      jobject JValue = getJValue(env, response);
      assert (response != 0);
      /*
       *  Single read response messages have a value pointer. Before freeing the memory of the message
       *  itself, we'll have to free the memory of value first.
       */
      if (NULL != response->GetValue()) {
        delete response->GetValue();
        response->SetValue(NULL);
      }
      delete response;
      if (JValue == NULL) return NULL;
      return JValue;
    }
    case ROLLBACKMESSAGE : {
      RollbackMessage* rollbackMessage = static_cast<RollbackMessage*>(responseMessage);
      if (handleRollback(env, refObject, rollbackMessage, agentID)) {
        assert (rollbackMessage != 0);
        delete rollbackMessage;
        fIAgent->SignalReceiveProcess();
        fIAgent->Unlock();
        fIAgent->PostReceiveWait();
        fIAgent->Lock();
        return readResponse(env, refObject, fIAgent->WaitForMessage(), agentID);
      }
      assert (rollbackMessage != 0);
      delete rollbackMessage;
      return NULL;
    }
    default :
      LOG(logERROR) << "Interface::readResponse(" << fCommRank << ")# Out of options, returning NULL, responseMessage: " << *responseMessage;
      exit(0);
  }
}

jboolean writeResponse(JNIEnv* env, jobject refObject, AbstractMessage* responseMessage, jlong agentID) {
  switch (responseMessage->GetType()) {
    case WRITERESPONSEMESSAGE : {
      WriteResponseMessage* response = static_cast<WriteResponseMessage*>(responseMessage);
      bool writeResponse = getWriteResponse(response);
      assert (response != 0);
      delete response;
      return writeResponse;
    }
    case ROLLBACKMESSAGE : {
      RollbackMessage* rollbackMessage = static_cast<RollbackMessage*>(responseMessage);
      if (handleRollback(env, refObject, rollbackMessage, agentID)) {
        assert (rollbackMessage != 0);
        delete rollbackMessage;
        fIAgent->SignalReceiveProcess();
        fIAgent->Unlock();
        fIAgent->PostReceiveWait();
        fIAgent->Lock();
        return writeResponse(env, refObject, fIAgent->WaitForMessage(), agentID);
      }
      assert (rollbackMessage != 0);
      delete rollbackMessage;
      return false;
    }
    default :
      LOG(logERROR) << "Interface::writeResponse(" << fCommRank << ")# Out of options, returning false, responseMessage: " << *responseMessage;
      exit(0);
  }
}

jobject rangeQueryResponse(JNIEnv* env, jobject refObject, AbstractMessage* responseMessage, jlong agentID) {
  switch (responseMessage->GetType()) {
    case RANGEQUERYMESSAGE : {
      RangeQueryMessage* rangeQueryMessage = static_cast<RangeQueryMessage*>(responseMessage);
      jobject JHashMap = getJHashMap(env, rangeQueryMessage);
      assert (rangeQueryMessage != 0);
      delete rangeQueryMessage;
      if (JHashMap == NULL) return NULL;
      return JHashMap;
    }
      break;
    case ROLLBACKMESSAGE : {
      RollbackMessage* rollbackMessage = static_cast<RollbackMessage*>(responseMessage);
      if (handleRollback(env, refObject, rollbackMessage, agentID)) {
        assert (rollbackMessage != 0);
        delete rollbackMessage;
        fIAgent->SignalReceiveProcess();
        fIAgent->Unlock();
        fIAgent->PostReceiveWait();
        fIAgent->Lock();
        return rangeQueryResponse(env, refObject, fIAgent->WaitForMessage(), agentID);
      }
      assert (rollbackMessage != 0);
      delete rollbackMessage;
      return NULL;
    }
      break;
    default :
      LOG(logERROR) << "Interface::rangeQuery(" << fCommRank << ")# Out of options, returning NULL, responseMessage: " << *responseMessage;
      exit(0);
  }
}

Point* getPoint(JNIEnv* env, jobject locationObject) {
	jclass locationClass = env->GetObjectClass(locationObject);
	if (locationClass == NULL) return NULL;
	jmethodID getXMID = env->GetMethodID(locationClass, "getX", "()I");
	if (getXMID == NULL) return NULL;
	jmethodID getYMID = env->GetMethodID(locationClass, "getY", "()I");
	if (getYMID == NULL) return NULL;
	jint xLocation = env->CallIntMethod(locationObject, getXMID);
	jint yLocation = env->CallIntMethod(locationObject, getYMID);
	return new Point(xLocation, yLocation);
}

bool handleRollback(JNIEnv* env, jobject refObject, const RollbackMessage* rollbackMessage, jlong agentID) {
    jlong rollbackAgentID = (jlong) rollbackMessage->GetOriginalAlp().GetId();
    jint rollbackTime = (jint) rollbackMessage->GetTimestamp();
    if (agentID == rollbackAgentID) {
      fIAgent->SetIgnoreID();
      throwRollbackException(env, rollbackAgentID, rollbackTime);
      return false;
    }
    rollbackCallback(env, refObject, rollbackAgentID, rollbackTime);
    return true;
}

void rollbackCallback(JNIEnv* env, jobject refObject, jlong rollbackAgentId, jint rollbackTime) {
    jclass distributedSSVHandlerClass = env->GetObjectClass(refObject);
    if (distributedSSVHandlerClass == NULL) return;
    jmethodID rollbackHandlerMID = env->GetMethodID(distributedSSVHandlerClass, "handleRollback", "(JI)V");
    if (rollbackHandlerMID == NULL) return;
    env->CallVoidMethod(refObject, rollbackHandlerMID, rollbackAgentId, rollbackTime);
}

void throwRollbackException(JNIEnv* env, jlong rollbackAgentId, jint rollbackTime) {
	jclass rollbackExceptionClass = env->FindClass("mwgrid/middleware/exception/RollbackException");
	if (rollbackExceptionClass == NULL) return;
	jmethodID rollbackExceptionMID = env->GetMethodID(rollbackExceptionClass, "<init>", "(JI)V");
	if (rollbackExceptionMID == NULL) return;
	jthrowable rollbackException = static_cast<jthrowable>(env->NewObject(rollbackExceptionClass, rollbackExceptionMID, rollbackAgentId, rollbackTime));
	env->Throw(rollbackException);
	env->DeleteLocalRef(rollbackExceptionClass);
}

jobject getJInteger(JNIEnv* env, int returnValue) {
	jclass integerClass = env->FindClass("java/lang/Integer");
	if (integerClass == NULL) return NULL;
	jmethodID integerConstructor = env->GetMethodID(integerClass, "<init>", "(I)V");
	if (integerConstructor == NULL) return NULL;
	return env->NewObject(integerClass, integerConstructor, returnValue);
}

jobject getJLong(JNIEnv* env, long returnValue) {
  jclass longClass = env->FindClass("java/lang/Long");
  if (longClass == NULL) return NULL;
  jmethodID longConstructor = env->GetMethodID(longClass, "<init>", "(J)V");
  if (longConstructor == NULL) return NULL;
  return env->NewObject(longClass, longConstructor, (long long) returnValue);
}

jobject getJDouble(JNIEnv* env, double returnValue) {
	jclass doubleClass = env->FindClass("java/lang/Double");
	if (doubleClass == NULL) return NULL;
	jmethodID doubleConstructor = env->GetMethodID(doubleClass, "<init>", "(D)V");
	if (doubleConstructor == NULL) return NULL;
	return env->NewObject(doubleClass, doubleConstructor, returnValue);
}

jobject getJLocation(JNIEnv* env, const Point returnValue) {
	jclass pointClass = env->FindClass("mwgrid/middleware/distributedobject/Location");
	if (pointClass == NULL) return NULL;
	jmethodID pointConstructor = env->GetMethodID(pointClass, "<init>", "(II)V");
	if (pointConstructor == NULL) return NULL;
	return env->NewObject(pointClass, pointConstructor, returnValue.GetX(), returnValue.GetY());
}

jobject getJString(JNIEnv* env, const string returnValue) {
	jstring returnString = env->NewStringUTF(returnValue.c_str());
	jclass stringClass = env->FindClass("java/lang/String");
	if (stringClass == NULL) return NULL;
	jmethodID stringConstructor = env->GetMethodID(stringClass, "<init>", "(Ljava/lang/String;)V");
	if (stringConstructor == NULL) return NULL;
	jobject stringObject = env->NewObject(stringClass, stringConstructor, returnString);
	return stringObject;
}

jobject getJValue(JNIEnv* env, const SingleReadResponseMessage* response) {
	const AbstractValue* responseValue = response->GetValue();
	jclass valueClass = env->FindClass("mwgrid/middleware/distributedobject/Value");
	if (valueClass == NULL) return NULL;
	jmethodID valueConstructor = env->GetMethodID(valueClass, "<init>", "(Ljava/lang/Object;)V");
	if (valueConstructor == NULL) return NULL;
	switch(responseValue->GetType()) {
	  case VALUEINT : {
        int returnValue = static_cast<const Value<int>* >(responseValue)->GetValue();
        jobject JInteger = getJInteger(env, returnValue);
        if (JInteger == NULL) return NULL;
        return env->NewObject(valueClass, valueConstructor, JInteger);
	  }
	  case VALUEDOUBLE : {
        double returnValue = static_cast<const Value<double>* >(responseValue)->GetValue();
        jobject JDouble = getJDouble(env, returnValue);
        if (JDouble == NULL) return NULL;
        return env->NewObject(valueClass, valueConstructor, JDouble);
	  }
	  case VALUEPOINT : {
        Point returnValue = static_cast<const Value<Point>* >(responseValue)->GetValue();
        jobject JLocation = getJLocation(env, returnValue);
        if (JLocation == NULL) return NULL;
        return env->NewObject(valueClass, valueConstructor, JLocation);
	  }
	  case VALUESTRING : {
        string returnValue = static_cast<const Value<string>* >(responseValue)->GetValue();
        jobject JString = getJString(env, returnValue);
        if (JString == NULL) return NULL;
        return env->NewObject(valueClass, valueConstructor, JString);
	  }
	  default :
	    LOG(logERROR) << "Interface::getJValue# Unrecognised value type received! " << *responseValue;
	    exit(1);
	}
}

jboolean getWriteResponse(const WriteResponseMessage* response) {
	WriteStatus writeStatus = response->GetWriteStatus();
	if(writeStatus == 0)
		return true;
	else
		return false;
}

jobject getJHashMap(JNIEnv* env, const RangeQueryMessage* response) {
	SerialisableMap<SsvId, Value<Point> > ssvValueList = response->GetSsvValueList();
	jclass mapClass = env->FindClass("java/util/HashMap");
	if (mapClass == NULL) return NULL;
	jmethodID mapConstructor = env->GetMethodID(mapClass, "<init>", "()V");
	if (mapConstructor == NULL) return NULL;
	jobject mapObject = env->NewObject(mapClass, mapConstructor);
	jmethodID putMethodID = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
	if (putMethodID == NULL) return NULL;
	SerialisableMap<SsvId, Value<Point> >::const_iterator iter;
	for(iter = ssvValueList.begin(); iter != ssvValueList.end(); ++iter) {
	    SsvId key = (*iter).first;
		long objectId = key.GetObjectId();
		Value<Point> value = (*iter).second;
		Point point = value.GetValue();
		LOG(logFINEST) << "Interface::getJHashMap(" << fCommRank << ")# point: " << point;
		jobject jObjectId = getJLong(env, objectId);
		if (jObjectId == NULL) return NULL;
		jobject jLocation = getJLocation(env, point);
		if (jLocation == NULL) return NULL;
		env->CallObjectMethod(mapObject, putMethodID, jObjectId, jLocation);
	}
	return mapObject;
}
