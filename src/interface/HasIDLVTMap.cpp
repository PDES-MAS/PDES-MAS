/*
 * HasIDLVTMap.cpp
 *
 *  Created on: 31 Jan 2011
 *      Author: Dr B.G.W. Craenen <b.g.w.craenen@cs.bham.ac.uk>
 */

#include <algorithm>
#include "limits.h"
#include "HasIDLVTMap.h"
#include "Log.h"
#include "spdlog/spdlog.h"
using namespace pdesmas;

void HasIDLVTMap::SetAgentLVT(long pAgentID, unsigned long pTime) {
  fLastAgentID = pAgentID;
  // Agent not yet in read LVT map
  if (!HasAgentID(pAgentID)) {
    fAgentIdLvtMap[pAgentID] = pTime;
    return;
  }
  // LVT already stored
  if (pTime == fAgentIdLvtMap[pAgentID]) {
    return;
  }
  // LVT smaller
  if (pTime < fAgentIdLvtMap[pAgentID]) {
    LOG(logWARNING) << "HasIDLVTMap::SetAgentLVT# Time to set smaller then stored LVT, agent: " << fLastAgentID << ", LVT: " << fAgentIdLvtMap[pAgentID] << ", time: " << pTime;
    return;
  }
  // Store LVT
  fAgentIdLvtMap[pAgentID] = pTime;
}

void HasIDLVTMap::SetAgentReadLVT(long pAgentID, unsigned long pTime) {
  SetAgentLVT(pAgentID, pTime);
}

void HasIDLVTMap::SetAgentWriteLVT(long pAgentID, unsigned long pTime) {
  SetAgentLVT(pAgentID, (pTime - 1));
}

long HasIDLVTMap::GetLastLVTAgentID() const {
  return fLastAgentID;
}

bool HasIDLVTMap::HasAgentID(long pAgentID) const {
  map<long, unsigned long>::const_iterator iter = fAgentIdLvtMap.find(pAgentID);
  if (iter != fAgentIdLvtMap.end()) return true;
  return false;
}

unsigned long HasIDLVTMap::GetAgentLVT(long pAgentID) const {
  map<long, unsigned long>::const_iterator iter = fAgentIdLvtMap.find(pAgentID);
  if (iter != fAgentIdLvtMap.end()) return iter->second;
  LOG(logERROR) << "HasIDLVTMap::GetAgentLVT# Couldn't get agent LVT for agent ID: " << pAgentID;
  return -1;
}

void HasIDLVTMap::RollbackAgentLVT(long pAgentID, unsigned long pTime) {
  if (!HasAgentID(pAgentID)) {
    LOG(logERROR) << "HasIDLVTMap::RollbackAgentLVT# Unknown agent: " << pAgentID;
    exit(1);
  }
  unsigned long time = pTime - 1;
  if (time < 0) {
    LOG(logERROR) << "HasIDLVTMap::RollbackAgentLVT# Rollback time smaller then 0, agent: " << pAgentID << ", LVT: " << fAgentIdLvtMap[pAgentID] << ", rollback time: " << pTime;
    exit(1);
  }
  if (time > fAgentIdLvtMap[pAgentID]) {
    LOG(logERROR) << "HasIDLVTMap::RollbackAgentLVT# Rollback time not smaller then LVT, agent: " << pAgentID << ", LVT: " << fAgentIdLvtMap[pAgentID] << ", rollback time: " << pTime;
    //exit(1);
  }
  fAgentIdLvtMap[pAgentID] = time;
}

unsigned long HasIDLVTMap::GetLVT() const {
  unsigned long result = ULONG_MAX;
  for (map<long, unsigned long>::const_iterator iter = fAgentIdLvtMap.begin(); iter != fAgentIdLvtMap.end(); ++iter) {
    LOG(logFINEST) << "HasIDLVTMap::GetLVT# Agent: " << iter->first << ", now at LVT: " << iter->second;
    if (iter->second < result) result = iter->second;
  }
  LOG(logFINEST) << "HasIDLVTMap::GetLVT# ALP LVT now at: " << result;
  return result;
}
