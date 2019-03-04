#include "ObjectMgr.h"

using namespace std;

namespace pdesmas {

  MessageClassMap* messageClassMap = MessageClassMap::Instance();
  ValueClassMap* valueClassMap = ValueClassMap::Instance();

  string GetValueString(string msgString) {
    size_t start = 0;
    size_t end = 0;
    start = msgString.find_first_not_of(DELIM_LEFT);

    if (start != string::npos) start = msgString.find_first_of(
        DELIM_VAR_SEPARATOR, start + 1);
    start += 1;
    if (start != string::npos) end = msgString.find_first_of(DELIM_RIGHT,
        start);

    if (end == string::npos) end = msgString.length();
    return (msgString.substr(start, end - start));
  }

}
