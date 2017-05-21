#ifndef NETMON_DATA_H
#define NETMON_DATA_H

#include "conf.h"
#include <chrono>

class Service {
 public:
  string name;
  // Control
  string startCmd;
  string stopCmd;
  string restartCmd;
  string reloadCmd;
  // Status
  string verifyCmd;
  string runningResultRegex;
  string stoppedResultRegex;
};

class IP {
  unsigned char ip[4];
 public:
  string asString();
  unsigned char* asBytes();

  IP();
  IP(string);
};

class Server {
 public:
  vector <Service> services;
  string name;
  IP ip;

  Server(Element*);
};

class Host {
public:
  Host(Element*);

  string hostname;
  string user;
  IP ip;
  bool up;

  void ping();

  bool pinging;
  chrono::system_clock::time_point pingTime;
  chrono::system_clock::time_point replyTime;
};

class Section {
 public:
  Section(Element*);
  ~Section();

  unsigned int up();

  string name;
  vector <Host*> hosts;
};

class Network {
 public:
  string name;
  vector<Server*> servers;
  vector<Section*> sections;

  Network(Configuration*);
  ~Network();

  unsigned int up();
 private:
  Element* getServers(Element*);
  Element* getSections(Element*);
};

#endif
