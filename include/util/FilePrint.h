/*
 * FilePrint.h
 *
 *  Created on: 5 Apr 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@xs4all.nl>
 */

#ifndef FILEPRINT_H_
#define FILEPRINT_H_

#include <string>

using std::string;

namespace pdesmas {
  enum FILENAME {
    TEST,
    IALPRECEIVE1,
    IALPRECEIVE2,
    IALPRECEIVE3,
    IALPRECEIVE4,
    IALPRECEIVE5,
    IALPSEND1,
    IALPSEND2,
    INTERFACE
  };

  class FilePrint {
    private:
      static unsigned long fInitialTime;
      static string ConvertFilename(int, FILENAME);
    public:
      FilePrint();
      virtual ~FilePrint();

      static void PrintToFile(int, FILENAME, string);
  };
}

#endif /* FILEPRINT_H_ */
