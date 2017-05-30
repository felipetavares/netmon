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

class MAC {
  unsigned char mac[6];
 public:
  string asString();
  unsigned char* asBytes();

  MAC();
  MAC(string);
};

class Server {
 public:
  vector <Service> services;
  string name;
  string username;
  IP ip;

  Server(Element*);
  void write(ostream&);
};

class Host {
public:
  Host(Element*);

  string hostname;
  string user;
  IP ip;
  MAC mac;
  bool up;

  void ping();
  void write(ostream&);

  bool pinging;
  chrono::system_clock::time_point pingTime;
  chrono::system_clock::time_point replyTime;
};

class Section {
 public:
  Section(string);
  Section(Element*);
  ~Section();

  unsigned int up();

  void addUpdater(unsigned long);
  void removeUpdaters();
  void write(ostream&);

  string name;
  vector <Host*> hosts;
  vector <unsigned long> updaters;
};

class Network {
 public:
  string name;
  vector<Server*> servers;
  vector<Section*> sections;

  Network(Configuration*);
  ~Network();

  unsigned int up();

  void write(ostream&);
 private:
  Element* getServers(Element*);
  Element* getSections(Element*);
};

#endif
