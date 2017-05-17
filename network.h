#ifndef NETMON_DATA_H
#define NETMON_DATA_H

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
  char[4] asBytes();
};

class Server {
 public:
  vector <Service> services;
  string name;
  IP ip;
};

class Host {
public:
  string hostname;
  string userName;
  IP ip;
};

class Section {
 public:
  string name;
  vector <Host*> hosts;
};

class Network {
 public:
  string name;
  vector<Server*> servers;
  vector<Section*> sections;

  void fillData(Configuration*);
};
