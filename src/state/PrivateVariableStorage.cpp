//
// Created by pill on 19-5-2.
//

#include "PrivateVariableStorage.h"

void PrivateVariableStorage::AddVariable(const SsvId &variable_id, AbstractValue *v, unsigned long timestamp) {
  if (variable_id_wp_map_.count(variable_id.id()) == 0) {
    return;
  }
  variable_id_wp_map_[variable_id.id()] = SerialisableList<WritePeriod>();
  variable_id_wp_map_[variable_id.id()].push_back(WritePeriod(v, timestamp, LpId()));

}

AbstractValue *PrivateVariableStorage::ReadVariable(const SsvId &variable_id, unsigned long timestamp) {
  if (variable_id_wp_map_.count(variable_id.id()) == 0) {
    return nullptr;
  }
  auto wp_list = variable_id_wp_map_[variable_id.id()];
  for (auto &i : wp_list) {
    if (i.GetStartTime() >= timestamp && i.GetEndTime() < timestamp) {
      return i.GetValueCopy();
    }
  }
  return nullptr;
}

void PrivateVariableStorage::WriteVariable(const SsvId &variable_id, AbstractValue *value, unsigned long timestamp) {
// no rollback will happen here since inside agent event must not arrive out of order
  if (variable_id_wp_map_.count(variable_id.id()) == 0) {
    return;
  }
  auto wp_list = variable_id_wp_map_[variable_id.id()];
  if (timestamp <= wp_list.rbegin()->GetStartTime()) {
    return;
  }
  wp_list.rbegin()->SetEndTime(timestamp);
  wp_list.push_back(WritePeriod(value, timestamp, LpId()));
}

void PrivateVariableStorage::PerformRollback(unsigned long timestamp) {
  for (const auto &i:variable_id_wp_map_) {
    auto wp_list = i.second;
    auto iter = wp_list.begin();
    while (iter != wp_list.end()) {
      if (iter->GetStartTime() < timestamp) {
        iter = wp_list.erase(iter);

      } else {
        ++iter;
      }
    }
  }
}

void PrivateVariableStorage::CleanUp(unsigned long timestamp) {
  for (const auto &i:variable_id_wp_map_) {
    auto wp_list = i.second;
    auto iter = wp_list.begin();
    while (iter != wp_list.end()) {
      if (iter->GetEndTime() <= timestamp) {
        iter = wp_list.erase(iter);

      } else {
        ++iter;
      }
    }
  }
}