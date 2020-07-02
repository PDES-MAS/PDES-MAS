//
// Created by huazh on 1/8/2020.
//

#include "NaiveAgent.h"

NaiveAgent::NaiveAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId)
    : Agent(
    startTime, endTime, agentId) {
}

void NaiveAgent::Cycle() {
  if (CheckSyncGVT()) {
    SendGVTMessage();
  }
  struct timeval start;
  struct timeval end;

  srand(agent_id() * GetLVT());

  for (auto &i:sendList) {
    int msgSerial = rand() % 100000;
    spdlog::debug("Msg serial of agent {0} is {1}", agent_id(), msgSerial);
    //tv_iter++;
    // spdlog::debug("send to {0}", i);
    // auto msgSerial = distribution(eg);
    string mailContent = "0-" + to_string(msgSerial);
    // i is receiver id, 0 means direction
    // bool tie = this->WriteMbString(i, mailContent, this->GetLVT() + 1);
    gettimeofday(&start, NULL);
    bool tie = this->SendMail(i, this->GetLVT() + 1, mailContent);
    gettimeofday(&end, NULL);
    // call ALP to send msg
    if (tie) {
      // received response msg
      spdlog::warn("LOGSEND {},{},{}", agent_id(), msgSerial,
                   1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, at ts = {3}, sent", this->agent_id(), i, msgSerial, this->GetLVT());
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write success", this->agent_id(), i, msgSerial);
    } else {
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, at ts = {3}, sent", this->agent_id(), i, msgSerial, this->GetLVT());
      spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write failed", this->agent_id(), i, msgSerial);
    }
  }

  spdlog::debug("Agent{0}, request to read mailbox", this->agent_id());
  gettimeofday(&start, NULL);
  list<Mail> newMails = ReadMail(this->GetLVT() + 1);
  gettimeofday(&end, NULL);
  spdlog::warn("LOGREAD {},{}", agent_id(),
               1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
  string delimiter = "-";
  spdlog::debug("Agent{0}, read mailbox success, size {1}", this->agent_id(), newMails.size());
  auto newMailIterator = newMails.begin();
  while (newMailIterator != newMails.end()) {
    unsigned long sender = newMailIterator->sender_;
    string mailContent = newMailIterator->message_;
    auto reply_tie = mailContent.substr(0, mailContent.find(delimiter));
    string serial_str = mailContent.erase(0, mailContent.find(delimiter) + delimiter.length());

    if (reply_tie == "0") {
      // generate reply
      string replyMsgContent = "1-" + serial_str;
      spdlog::debug("Agent{0},request to reply Agent{1},serial{2}", agent_id(), sender, stoi(serial_str));
      gettimeofday(&start, NULL);
      bool replyTie = this->SendMail(sender, this->GetLVT() + 1, replyMsgContent);
      gettimeofday(&end, NULL);

      if (replyTie) {
        // received response msg
        spdlog::warn("LOGSEND {},{},{}", agent_id(), serial_str,
                     1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec);
        spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write success", this->agent_id(), sender, stoi(serial_str));
      } else {
        spdlog::debug("Agent{0}, Agent{1}, MsgID{2}, write failed", this->agent_id(), sender, stoi(serial_str));
      }
    } else if (reply_tie == "1") {
      // nothing
    }
    newMailIterator++;
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

list<Mail> NaiveAgent::ReadMail(unsigned long timestamp) {
  list<Mail> mailList = list<Mail>();
  //read first
  string current = this->ReadString(this->agent_id(), timestamp);
  //get mails and write back
  spdlog::debug("NaiveAgent::ReadMail: raw string {}", current);
  list<Mail> allMails = ParseMailListFromString(current);

  return allMails;
}


list<Mail> NaiveAgent::ParseMailListFromString(string content) {
  size_t pos = 0;
  list<Mail> mailList = list<Mail>();
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

string NaiveAgent::MailListToString(list<Mail> mailList) {
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

bool NaiveAgent::CheckSyncGVT() {
  return true;
}
