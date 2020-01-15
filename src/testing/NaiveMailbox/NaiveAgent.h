
#ifndef PDES_MAS_NAIVEAGENT_H
#define PDES_MAS_NAIVEAGENT_H

#include "Agent.h"

#include <utility>

string const MAIL_DELIM = "#";
string const MAIL_INTERNAL_DELIM = "@";
using namespace pdesmas;

class Mail {
public:

  unsigned long sender_;
  unsigned long timestamp_;
  string message_;

  Mail(unsigned long sender, unsigned long timestamp, string message) : sender_(sender), timestamp_(timestamp),
                                                                        message_(std::move(message)) {};
};

class NaiveAgent : public Agent {

public:
  NaiveAgent(const unsigned long startTime, const unsigned long endTime, unsigned long agentId);

  void Cycle() override;

  void InitSendList(list<unsigned long> agList, unsigned int listLen, unsigned int seed);


private:
  list<unsigned long> sendList;

  // MA INTERFACE: if true, initiate GVT sync. Flag will be automatically cleared
  bool CheckSyncGVT();

  // MA INTERFACE: send a mail (string content)
  bool SendMail(unsigned long agentId, unsigned long timestamp, string mailContent);

  // MA INTERFACE: receive mail
  list <Mail> ReadMail(unsigned long timestamp);

  list <Mail> ParseMailListFromString(string content);

  Mail ParseMailFromString(string content);

  string MailListToString(list <Mail> mailList);

  string MailToString(Mail &mail);
};


#endif //PDES_MAS_NAIVEAGENT_H
