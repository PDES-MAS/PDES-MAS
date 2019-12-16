/*
 * Initialisor.cpp
 *
 *  Created on: 18 Aug 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "SingleReadMessage.h"
#include "SingleReadResponseMessage.h"
#include "SingleReadAntiMessage.h"
#include "RangeQueryMessage.h"
#include "RangeQueryAntiMessage.h"
#include "WriteMessage.h"
#include "WriteResponseMessage.h"
#include "WriteAntiMessage.h"
#include "GvtControlMessage.h"
#include "GvtRequestMessage.h"
#include "GvtValueMessage.h"
#include "RollbackMessage.h"
#include "StateMigrationMessage.h"
#include "RangeUpdateMessage.h"
#include "EndMessage.h"
#include "Initialisor.h"
#include "Log.h"
#include "Clp.h"
#include <spdlog/spdlog.h>

#define MAX(a, b) (a>b?a:b)
#define MIN(a, b) (a<b?a:b)
using namespace std;
using namespace pdesmas;

Initialisor::Initialisor()
    : fHasInitInt(false), fHasInitLong(false), fHasInitDouble(false), fHasInitPoint(false), fHasInitString(false) {

}


Initialisor::~Initialisor() {
  fClpIdRangeMap.clear();
  fClpIdSsvIdMap.clear();
  fAlpToClpMap.clear();
  fClpSsvIdValueMap.clear();
}

void Initialisor::attach_alp_to_clp(int alp, int clp) {
  fAlpToClpMap[alp] = clp;
}

void Initialisor::preload_variable(const string &type, unsigned long variable_id, const string &v, unsigned int clpId) {
  AbstractValue *value;
  if (type.compare("INT") == 0) {
    value = valueClassMap->CreateObject(VALUEINT);
    value->SetValue(v);
  } else if (type.compare("LONG") == 0) {
    value = valueClassMap->CreateObject(VALUELONG);
    value->SetValue(v);
  } else if (type.compare("DOUBLE") == 0) {
    value = valueClassMap->CreateObject(VALUEDOUBLE);
    value->SetValue(v);
  } else if (type.compare("POINT") == 0) {
    value = valueClassMap->CreateObject(VALUEPOINT);
    value->SetValue(v);
  } else if (type.compare("STRING") == 0) {
    value = valueClassMap->CreateObject(VALUESTRING);
    value->SetValue(v);
  } else {
    LOG(logERROR) << "Initialisor::ParseSSV# Unrecognised SSV type: " << type;
    return;
  }
  auto ssvID = SsvId(variable_id);
  fClpSsvIdValueMap.insert(make_pair(ssvID, value));
  if (fClpIdSsvIdMap.find(clpId) == fClpIdSsvIdMap.end()) {
    fClpIdSsvIdMap.insert(make_pair(clpId, list<SsvId>()));
  }
  fClpIdSsvIdMap[clpId].push_back(ssvID);

  if (type.compare("POINT") == 0) {
    // update range
    Point pv = ((Value<Point> *) value)->GetValue();

    if (fClpIdRangeMap.find(clpId) == fClpIdRangeMap.end()) {
      fClpIdRangeMap[clpId] = Range(pv, pv);
    } else {
      Point max = fClpIdRangeMap[clpId].GetMaxRangeValue();
      Point min = fClpIdRangeMap[clpId].GetMinRangeValue();
      Point newMax, newMin;

      //see if the point is the new min/max
      if (max.GetX() < pv.GetX() || max.GetY() < pv.GetY()) {
        fClpIdRangeMap[clpId].SetMaxRangeValue(Point(MAX(max.GetX(), pv.GetX()), MAX(max.GetY(), pv.GetY())));
      } else if (min.GetX() > pv.GetX() || max.GetY() > pv.GetY()) {
        fClpIdRangeMap[clpId].SetMinRangeValue(Point(MIN(min.GetX(), pv.GetX()), MIN(min.GetY(), pv.GetY())));

      }


    }
  }
}

void Initialisor::ParseFileCLP(const string pFileName, int pClpNumber) {
  ifstream file(pFileName.c_str());
  if (!file) {
    LOG(logERROR) << "Initialisor::ParseFile# Could not open initialisation file: " << pFileName;
    exit(1);
  }

  string buffer;
  while (!file.eof()) {
    getline(file, buffer);
    // Skip empty lines
    if (!buffer.empty()) {
      if (buffer.compare(0, 9, "message: ") == 0) {
        ParseMessage(buffer);
      } else if (buffer.compare(0, 5, "alp: ") == 0) {
        ParseALP(buffer);
      } else if (buffer.compare(0, 5, "ssv: ") == 0) {
        ParseSSV(buffer, pClpNumber);
      } else if (buffer.compare(0, 5, "clp: ") == 0) {
        ParseCLP(buffer);
      } else {
        LOG(logWARNING) << "Initialisor::ParseFile# Unrecognised line in initialisation file: " << buffer;
      }
    }
  }
  file.close();
}

void Initialisor::ParseFileALP(const string pFileName) {
  ifstream file(pFileName.c_str());
  if (!file) {
    LOG(logERROR) << "Initialisor::ParseFile# Could not open initialisation file: " << pFileName;
    exit(1);
  }

  string line;
  while (!file.eof()) {
    getline(file, line);
    // Skip empty lines
    if (!line.empty()) {
      if (line.compare(0, 9, "message: ") == 0) {
        // Initialise the messages
        ParseMessage(line);
      } else if (line.compare(0, 5, "alp: ") == 0) {
        // Initialise the ALP to CLP table
        ParseALP(line);
      } else if (line.compare(0, 5, "ssv: ") == 0) {
        // Parse the SSVs for their types
        ParseSSVForALP(line);
      } else if (line.compare(0, 5, "clp: ") == 0) {
        // Skip this line for the ALP
      } else {
        LOG(logWARNING) << "Initialisor::ParseFile# Unrecognised line in initialisation file: " << line;
      }
    }
  }
  file.close();
}

const map<unsigned int, Range> &Initialisor::GetClpToRangeMap() const {
  return fClpIdRangeMap;
}

const map<unsigned int, list<SsvId> > &Initialisor::GetClpToSsvMap() const {
  return fClpIdSsvIdMap;
}

const map<unsigned int, unsigned int> &Initialisor::GetAlpToClpMap() const {
  return fAlpToClpMap;
}

const map<SsvId, AbstractValue *> &Initialisor::GetClpSsvIdValueMap() const {
  return fClpSsvIdValueMap;
}

void Initialisor::ParseMessage(const string pLine) const {
  string messageName = pLine.substr(9, pLine.size());
  if (messageName.compare("SingleReadMessage") == 0) {
    SingleReadMessage();
  } else if (messageName.compare("SingleReadResponseMessage") == 0) {
    SingleReadResponseMessage();
  } else if (messageName.compare("SingleReadAntiMessage") == 0) {
    SingleReadAntiMessage();
  } else if (messageName.compare("RangeQueryMessage") == 0) {
    RangeQueryMessage();
  } else if (messageName.compare("RangeQueryAntiMessage") == 0) {
    RangeQueryAntiMessage();
  } else if (messageName.compare("WriteMessage") == 0) {
    WriteMessage();
  } else if (messageName.compare("WriteResponseMessage") == 0) {
    WriteResponseMessage();
  } else if (messageName.compare("WriteAntiMessage") == 0) {
    WriteAntiMessage();
  } else if (messageName.compare("GvtControlMessage") == 0) {
    GvtControlMessage();
  } else if (messageName.compare("GvtRequestMessage") == 0) {
    GvtRequestMessage();
  } else if (messageName.compare("GvtValueMessage") == 0) {
    GvtValueMessage();
  } else if (messageName.compare("RollbackMessage") == 0) {
    RollbackMessage();
  } else if (messageName.compare("StateMigrationMessage") == 0) {
    StateMigrationMessage();
  } else if (messageName.compare("RangeUpdateMessage") == 0) {
    RangeUpdateMessage();
  } else if (messageName.compare("EndMessage") == 0) {
    EndMessage();
  } else {
    LOG(logERROR) << "Initialisor::ParseMessage# Unrecognised message type: " << messageName;
  }
}

void Initialisor::ParseALP(const string pLine) {
  string remainder = pLine.substr(5, pLine.size());
  size_t arrowPosition = remainder.find("->");
  unsigned int alpNumber = Helper::stream_cast<unsigned int, string>(remainder.substr(0, arrowPosition));
  unsigned int parentNumber = Helper::stream_cast<unsigned int, string>(
      remainder.substr(arrowPosition + 2, remainder.size()));
  fAlpToClpMap[alpNumber] = parentNumber;
}

void Initialisor::ParseSSV(const string pLine, int pClpRank) {
  string remainder = pLine.substr(5, pLine.size());
  size_t commaPosition = remainder.find(",");
  int clpNumber = Helper::stream_cast<int, string>(remainder.substr(0, commaPosition));
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  //long agentNumber = Helper::stream_cast<long, string>(remainder.substr(0, commaPosition));
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  int ssvNumber = Helper::stream_cast<int, string>(remainder.substr(0, commaPosition));
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  string ssvType = remainder.substr(0, commaPosition);
  InitType(ssvType);
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  string ssvValue = remainder.substr(0, remainder.size());
  // Add the SSV to the CLP
  SsvId ssvID = SsvId(ssvNumber);
  if (pClpRank == clpNumber) {
    //AbstractValue* value = valueClassMap->CreateObject(ssvType);
    // TODO: Nice this up
    AbstractValue *value;
    if (ssvType.compare("INT") == 0) {
      value = valueClassMap->CreateObject(VALUEINT);
      value->SetValue(ssvValue.substr(1, ssvValue.size() - 1));
    } else if (ssvType.compare("LONG") == 0) {
      value = valueClassMap->CreateObject(VALUELONG);
      value->SetValue(ssvValue.substr(1, ssvValue.size() - 1));
    } else if (ssvType.compare("DOUBLE") == 0) {
      value = valueClassMap->CreateObject(VALUEDOUBLE);
      value->SetValue(ssvValue.substr(1, ssvValue.size() - 1));
    } else if (ssvType.compare("POINT") == 0) {
      value = valueClassMap->CreateObject(VALUEPOINT);
      value->SetValue(ssvValue);
    } else if (ssvType.compare("STRING") == 0) {
      value = valueClassMap->CreateObject(VALUESTRING);
      value->SetValue(ssvValue.substr(1, ssvValue.size() - 1));
    } else {
      LOG(logERROR) << "Initialisor::ParseSSV# Unrecognised SSV type: " << ssvType;
      return;
    }
    fClpSsvIdValueMap.insert(make_pair(ssvID, value));
  }

  // Add SsvId to CLP to SsvId map
  map<unsigned int, list<SsvId> >::iterator mapIterator = fClpIdSsvIdMap.find(clpNumber);
  if (mapIterator == fClpIdSsvIdMap.end()) {
    // CLP not yet in the map
    list<SsvId> ssvIdList;
    ssvIdList.push_back(ssvID);
    fClpIdSsvIdMap.insert(make_pair(clpNumber, ssvIdList));
  } else {
    // CLP in the map
    mapIterator->second.push_back(ssvID);
  }
}

void Initialisor::ParseSSVForALP(const string pLine) {
  string remainder = pLine.substr(5, pLine.size());
  size_t commaPosition = remainder.find(",");
  //int clpNumber = Helper::stream_cast<int, string>(remainder.substr(0, commaPosition));
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  //int agentNumber = Helper::stream_cast<int, string>(remainder.substr(0, commaPosition));
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  //int ssvNumber = Helper::stream_cast<int, string>(remainder.substr(0, commaPosition));
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  string ssvType = remainder.substr(0, commaPosition);
  InitType(ssvType);
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  //string ssvValue = remainder.substr(0, remainder.size());
}

void Initialisor::ParseCLP(const string pLine) {
  string remainder = pLine.substr(5, pLine.size());
  size_t commaPosition = remainder.find(",");
  unsigned int clpNumber = Helper::stream_cast<unsigned int, string>(remainder.substr(0, commaPosition));
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  string minRange = remainder.substr(0, commaPosition);
  remainder = remainder.substr(commaPosition + 2, remainder.size());
  commaPosition = remainder.find(",");
  string maxRange = remainder.substr(0, remainder.size());
  Range range(Helper::stream_cast<Point>(minRange), Helper::stream_cast<Point>(maxRange));
  fClpIdRangeMap.insert(make_pair(clpNumber, range));
}

void Initialisor::InitType(const string pTypeString) {
  if (pTypeString.compare("INT") == 0 && !fHasInitInt) {
    Value<int>();
    fHasInitInt = true;
  } else if (pTypeString.compare("DOUBLE") == 0 && !fHasInitDouble) {
    Value<float>();
    fHasInitDouble = true;
  } else if (pTypeString.compare("POINT") == 0 && !fHasInitPoint) {
    Value<Point>();
    fHasInitPoint = true;
  } else if (pTypeString.compare("STRING") == 0 && !fHasInitString) {
    Value<string>();
    fHasInitString = true;
  } else if (pTypeString.compare("LONG") == 0 && !fHasInitLong) {
    Value<long>();
    fHasInitLong = true;
  }
}

void Initialisor::InitEverything() {

  Value<int>();
  fHasInitInt = true;

  Value<double>();
  fHasInitDouble = true;

  Value<Point>();
  fHasInitPoint = true;

  Value<string>();
  fHasInitString = true;

  Value<long>();
  fHasInitLong = true;


  SingleReadMessage();
  SingleReadResponseMessage();
  SingleReadAntiMessage();
  RangeQueryMessage();
  RangeQueryAntiMessage();
  WriteMessage();
  WriteResponseMessage();
  WriteAntiMessage();
  GvtControlMessage();
  GvtRequestMessage();
  GvtValueMessage();
  RollbackMessage();
  StateMigrationMessage();
  RangeUpdateMessage();
  EndMessage();
}


