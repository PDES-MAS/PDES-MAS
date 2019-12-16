#ifndef _Included_mwgrid_middleware_kernel_pdesmas_PDESMASInterface
#define _Included_mwgrid_middleware_kernel_pdesmas_PDESMASInterface

#include <jni.h>
#include "Agent.h"
#include "Clp.h"

#ifdef __cplusplus
extern "C" {
#endif
  pdesmas::Agent* fIAgent;
  pdesmas::Clp* fClp;
  int fCommRank;
  int fCommSize;
  int fNumberOfClps;
  int fNumberOfAlps;
  int fStartTime;
  int fEndTime;

  JNIEXPORT void
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_construct(
      JNIEnv *, jobject, jlong, jlong, jlong, jlong);

  JNIEXPORT void
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_initialise(
      JNIEnv *, jobject, jstring);

  JNIEXPORT void
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_finalise(
      JNIEnv *, jobject);

  JNIEXPORT jint
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_getRank(
      JNIEnv *, jobject);

  JNIEXPORT jint
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_getSize(
      JNIEnv *, jobject);

  JNIEXPORT jint
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_getGVT(
      JNIEnv *, jobject);

  JNIEXPORT void
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_sendGVTMessage(
      JNIEnv *, jobject);

  JNIEXPORT jobject
  JNICALL Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_read(
          JNIEnv *, jobject, jlong, jlong, jint, jint);

  JNIEXPORT jboolean
  JNICALL
      Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJIII(
          JNIEnv *, jobject, jlong, jlong, jint, jint, jint);

  JNIEXPORT jboolean
  JNICALL
      Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJIDI(
          JNIEnv *, jobject, jlong, jlong, jint, jdouble, jint);

  JNIEXPORT jboolean
  JNICALL
      Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJILmwgrid_middleware_distributedobject_Location_2I(
          JNIEnv *, jobject, jlong, jlong, jint, jobject, jint);

  JNIEXPORT jboolean
  JNICALL
      Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_write__JJILjava_lang_String_2I(
          JNIEnv *, jobject, jlong, jlong, jint, jstring, jint);

  JNIEXPORT jobject
  JNICALL
      Java_mwgrid_middleware_kernel_pdesmas_PDESMASInterface_rangeQuery(
          JNIEnv *, jobject, jlong, jint, jobject, jobject);

  bool checkOutsideRollback(JNIEnv*, jobject, jlong);
  jobject readResponse(JNIEnv*, jobject, AbstractMessage*, jlong);
  jboolean writeResponse(JNIEnv*, jobject, AbstractMessage*, jlong);
  jobject rangeQueryResponse(JNIEnv*, jobject, AbstractMessage*, jlong);

  Point* getPoint(JNIEnv*, jobject);
  bool handleRollback(JNIEnv*, jobject, const RollbackMessage*, jlong);
  void rollbackCallback(JNIEnv*, jobject, jlong, jint);
  void throwRollbackException(JNIEnv*, jlong, jint);
  jobject getJInteger(JNIEnv*, int);
  jobject getJLong(JNIEnv*, long);
  jobject getJDouble(JNIEnv*, double);
  jobject getJLocation(JNIEnv*, const Point );
  jobject getJString(JNIEnv*, const string);
  jobject getJValue(JNIEnv*, const SingleReadResponseMessage*);
  jboolean getWriteResponse(const WriteResponseMessage*);
  jobject getJHashMap(JNIEnv*, const RangeQueryMessage*);

#ifdef __cplusplus
}
#endif
#endif
