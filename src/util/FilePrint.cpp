/*
 * FilePrint.cpp
 *
 *  Created on: 5 Apr 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@xs4all.nl>
 */

#include "FilePrint.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Helper.h"

using namespace pdesmas;
using std::ofstream;
using std::ios;
using std::stringstream;
using std::endl;

unsigned long FilePrint::fInitialTime = Helper::GetTimeInMS();

FilePrint::FilePrint() {
  // Empty constructor
}

FilePrint::~FilePrint() {
  // Empty deconstructor
}

void FilePrint::PrintToFile(int pRank, FILENAME pFilename, string pLine) {
  ofstream outputFile;
  outputFile.open(ConvertFilename(pRank, pFilename).c_str(), ios::app);
  outputFile << pLine << endl;
  outputFile.close();
}

string FilePrint::ConvertFilename(int pRank, FILENAME pFilename) {
  stringstream result;
  result << "resources/" << fInitialTime << "-" << pRank << "-";
  switch (pFilename) {
    case TEST:
      result << "Test";
      break;
    case IALPRECEIVE1:
      result << "IALP_RECEIVE1";
      break;
    case IALPRECEIVE2:
      result << "IALP_RECEIVE2";
      break;
    case IALPRECEIVE3:
      result << "IALP_RECEIVE3";
      break;
    case IALPRECEIVE4:
      result << "IALP_RECEIVE4";
      break;
    case IALPRECEIVE5:
      result << "IALP_RECEIVE5";
      break;
    case IALPSEND1:
      result << "IALP_SEND1";
      break;
    case IALPSEND2:
      result << "IALP_SEND2";
      break;
    case INTERFACE:
      result << "Interface";
      break;
    default:
      result << "Default";
      break;
  }
  result << ".csv";
  return result.str();
}
