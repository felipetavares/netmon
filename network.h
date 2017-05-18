#ifndef NETMON_DATA_H
#define NETMON_DATA_H

#include "conf.h"

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
  char ip[4];
 public:
  string asString();
  char* asBytes();

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
};

class Section {
 public:
  Section(Element*);
  ~Section();

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
 private:
  Element* getServers(Element*);
  Element* getSections(Element*);
};

#endif
