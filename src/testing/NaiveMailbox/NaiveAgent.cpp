//
// Created by huazh on 1/8/2020.
//

#include "NaiveAgent.h"

NaiveAgent::NaiveAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId)
    : Agent(
    startTime, endTime, agentId) {
}

void NaiveAgent::Cycle() {
  spdlog::warn("Cycle begin, Agent {}, LVT {}, GVT {}", agent_id(), GetLVT(), GetGVT());
  this->SendGVTMessage();


  srand(agent_id() * GetLVT());
//  vector<int> temp_vector;
//  for (int i = 100000; i < 999999; i++) {
//    temp_vector.push_back(i);
//  }
//  random_shuffle(temp_vector.begin(), temp_vector.end());
//  auto tv_iter = temp_vector.begin();
  int success_count = 0;
  for (auto &i:sendList) {
    //int msgSerial = *tv_iter;
    int msgSerial = rand() % 100000;
    //tv_iter++;
    string message = "0-" + to_string(msgSerial);
    // send out
    bool success = SendMail(i, this->GetLVT(), message);
    spdlog::debug("Msg serial of agent {0} is {1}, send to {2}, success={3}", agent_id(), msgSerial, i, success);
    if (success) { ++success_count; }
  }
  spdlog::debug("Active sending done, success: {}/{}", success_count, sendList.size());

  this->time_wrap(rand() % 5 + 5);
  list <Mail> new_mails = ReadMail(this->GetLVT());
  spdlog::debug("{} new mails received", new_mails.size());
  string delimiter = "-";
  for (auto &j:new_mails) {
    string mail_content = j.message_;
    auto reply_tie = mail_content.substr(0, mail_content.find(delimiter));
    string serial_str = mail_content.erase(0, mail_content.find(delimiter) + delimiter.length());
    if (reply_tie == "0") {
      spdlog::debug("Agent{0}, request to reply msg from Agent{1},serial{2}", agent_id(), j.sender_, stoi(serial_str));
      string reply_content = "1-" + serial_str;;
      bool send_success = SendMail(j.sender_, this->GetLVT(), reply_content);
      if (send_success) {
        spdlog::debug("Agent{0}, reply to Agent{1} successfully", agent_id(), j.sender_);
      } else {
        spdlog::error("Agent{0}, reply to Agent{1} failed", agent_id(), j.sender_);
      }
    } else {
      spdlog::debug("Agent{0}, received reply from Agent{1}, serial{2}", agent_id(), j.sender_, stoi(serial_str));
    }
  }
}

bool NaiveAgent::SendMail(unsigned long agentId, unsigned long timestamp, string mailContent) {
  //read first
  string current = this->ReadString(agentId, timestamp);
  auto mailList = this->ParseMailListFromString(current);
  spdlog::debug("NaiveAgent::SendMail: size {}", mailList.size());
  mailList.emplace_back(Mail(this->agent_id(), timestamp, std::move(mailContent)));
  //write back
  string writeBack = this->MailListToString(mailList);

  return this->WriteString(agentId, writeBack, timestamp + 1);
}

list <Mail> NaiveAgent::ReadMail(unsigned long timestamp) {
  list <Mail> mailList = list<Mail>();
  //read first
  string current = this->ReadString(this->agent_id(), timestamp);
  //get mails and write back
  spdlog::debug("NaiveAgent::ReadMail: raw string {}", current);
  list <Mail> allMails = ParseMailListFromString(current);

  return allMails;
}


list <Mail> NaiveAgent::ParseMailListFromString(string content) {
  size_t pos = 0;
  list <Mail> mailList = list<Mail>();
  if (content.length() == 0) {
    return mailList;
  }
  if (content.find(MAIL_DELIM) == std::string::npos) {
    // not empty and no delim -> single piece of mail
    Mail mail = ParseMailFromString(content);
    mailList.push_back(mail);
    return mailList;
  }
  while ((pos = content.find(MAIL_DELIM)) != std::string::npos) {
    Mail mail = ParseMailFromString(content.substr(0, pos));
    content.erase(0, pos + MAIL_DELIM.length());
    mailList.push_back(mail);
  }
  return mailList;
}

Mail NaiveAgent::ParseMailFromString(string content) {
  unsigned long sender, timestamp;
  string message;
  size_t pos = 0;
  pos = content.find(MAIL_INTERNAL_DELIM);
  sender = std::stoul(content.substr(0, pos));
  content.erase(0, pos + MAIL_INTERNAL_DELIM.length());
  pos = content.find(MAIL_INTERNAL_DELIM);
  timestamp = std::stoul(content.substr(0, pos));
  content.erase(0, pos + MAIL_INTERNAL_DELIM.length());
  pos = content.find(MAIL_INTERNAL_DELIM);
  message = content.substr(0, pos);
  Mail m = Mail(sender, timestamp, message);
  return m;
}

string NaiveAgent::MailListToString(list <Mail> mailList) {
  string result;
  for (auto m:mailList) {
    result += this->MailToString(m);
    result += MAIL_DELIM;
  }
  result.erase(result.length() - 1, 1);
  return result;
}

void NaiveAgent::InitSendList(list<unsigned long> agList, unsigned int listLen, unsigned int seed) {
  list<unsigned long> rList;
  srand(seed * agent_id());
  for (unsigned int i = 0; i < listLen; ++i) {
    auto random_index = random() % agList.size();
    auto agListIter = agList.begin();
    advance(agListIter, random_index);
    if (*agListIter != agent_id()) {
      rList.push_back(*agListIter);
    }
  }
  sendList = rList;
}

string NaiveAgent::MailToString(Mail &m) {
  return to_string(m.sender_).append(MAIL_INTERNAL_DELIM).append(to_string(m.timestamp_)).append(
      MAIL_INTERNAL_DELIM).append(m.message_);
}
