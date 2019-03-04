/*
 * Helper.h
 *
 *  Created on: 18 Nov 2010
 *      Author: Dr B.G.W. Craenen (b.g.w.craenen@cs.bham.ac.uk)
 */

#ifndef HELPER_H_
#define HELPER_H_

#include <sstream>
#include <sys/time.h>

namespace Helper {
  template<typename inType>
  std::string string_cast(const inType& pInType) {
    std::ostringstream oss;
    oss << pInType;
    return oss.str();
  }

  template<typename outType, typename inType>
  outType stream_cast(const inType& pInType) {
    std::stringstream ss;
    outType requiredType; // value will be converted to outType

    ss << pInType; // insert the input value into a stream
    ss >> requiredType; // write value to reqType
    return requiredType;
  }

  inline unsigned long GetTimeInMS() {
    timeval timeValue;
    gettimeofday(&timeValue, NULL);
    return (((unsigned long int) timeValue.tv_sec % (unsigned long int) 1000000)
        * (unsigned long int) 1000) + ((unsigned long int) timeValue.tv_usec
        / (unsigned long int) 1000);
  }
}
#endif /* HELPER_H_ */
