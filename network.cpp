#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include "network.h"
#include "update.h"

// Network

Element* Network::getServers(Element *net) {
  for (auto c :net->children) {
    if (c->data == "SERVIDORES") {
      return c;
    }
  }

  return NULL;
}

Element* Network::getSections(Element *net) {
  for (auto c :net->children) {
    if (c->data == "SEÇÕES") {
      return c;
    }
  }

  return NULL;
}

Network::Network(Configuration *conf) {
  Element *root = conf->getRoot();

  if (root != NULL) {
    name = root->data;

    // Servers
    Element *rawServers = getServers(root);

    if (rawServers != NULL) {
      for (auto rawServer :rawServers->children) {
        servers.push_back(new Server(rawServer));
      }
    } else {
      cout << "Erro: arquivo de configuração não contém seção SERVIDORES." << endl;
    }
    // Sections
    Element *rawSections = getSections(root);

    if (rawSections != NULL) {
      for (auto rawSection :rawSections->children) {
        sections.push_back(new Section(rawSection));
      }
    } else {
      cout << "Erro: arquivo de configuração não contém seção SEÇÕES." << endl;
    }
  } else {
    cout << "Erro: arquivo de configuração não contém configuração de rede." << endl;
  }
}

Network::~Network() {
  for (auto server :servers) {
    delete server;
  }

  for (auto section :sections) {
    delete section;
  }
}

unsigned int Network::up () {
  unsigned int n = 0;

  for (auto s :sections) {
    n += s->up();
  }

  return n;
}

// Section

Section::Section (string name) {
  this->name = name;
}

Section::Section (Element *raw) {
  name = raw->data;

  for (auto c :raw->children) {
    hosts.push_back(new Host(c));
  }
}

Section::~Section () {
  for (auto h :hosts) {
    delete h;
  }
}

unsigned int Section::up () {
  unsigned int n = 0;

  for (auto h :hosts) {
    n += (unsigned int)h->up;
  }

  return n;
}

void Section::addUpdater (unsigned long updater) {
  updaters.push_back(updater);
}

void Section::removeUpdaters () {
  for (auto u :updaters) {
    update::remove(u);
  }
  updaters.clear();
}

// Host

Host::Host (Element *raw): up(false) {
  if (raw->children.size() != 3) {
    cout << "Erro: número de entradas em host inválido." << endl;
  } else {
    hostname = raw->data;

    user = raw->children[0]->data;
    ip = IP(raw->children[1]->data);
    mac = MAC(raw->children[2]->data);
  }
}

void Host::ping () {
  if (pinging)
    return;

  pinging = true;
  pingTime = chrono::system_clock::now();

  thread t([&]() {
    int ret = system(("ping "+ip.asString()+" -c 1 2>/dev/null 1>/dev/null").c_str());

    replyTime = chrono::system_clock::now();

    if (ret == 0) {
      up = true;
    } else {
      up = false;
    }

    pinging = false;
  });

  t.detach();
}

// MAC

MAC::MAC(string raw) {
  vector <unsigned int> positions;
  unsigned int rawSize = raw.size();

  positions.push_back(0);

  for (unsigned int i=0;i<rawSize;i++) {
    if(raw[i] == ':') {
      positions.push_back(i+1);
    }
  }

  if (positions.size() != 6) {
    cout << "Erro: MAC inválido." << endl;
    return;
  }

  for (unsigned int i=0;i<6;i++) {
    mac[i] = stoi(raw.substr(positions[i]), nullptr, 16);
  }
}

MAC::MAC () {

}

string MAC::asString() {
  stringstream maker;

  maker << std::setfill('0') << std::hex << std::setw(2);

  maker << (int)mac[0] << ':';
  maker << (int)mac[1] << ':';
  maker << (int)mac[2] << ':';
  maker << (int)mac[3];

  return maker.str();
}

unsigned char* MAC::asBytes() {
  return mac;
}

// IP

IP::IP(string raw) {
  vector<unsigned int> positions;
  unsigned int rawSize = raw.size();

  positions.push_back(0);

  for (unsigned int i=0;i<rawSize;i++) {
    if (raw[i] == '.') {
      positions.push_back(i+1);
    }
  }

  if (positions.size() != 4) {
    cout << "Erro: IP inválido." << endl;

    return;
  }

  for (unsigned int i=0;i<4;i++) {
    ip[i] = stoi(raw.substr(positions[i]), nullptr);
  }
}

IP::IP () {

}

string IP::asString() {
  stringstream maker;

  maker << (int)ip[0] << '.';
  maker << (int)ip[1] << '.';
  maker << (int)ip[2] << '.';
  maker << (int)ip[3];

  return maker.str();
}

unsigned char* IP::asBytes() {
  return ip;
}

// Server

Server::Server (Element* raw) {
  name = raw->data;

  if (raw->children.size() == 2) {
    username = raw->children[0]->data;
    ip = IP(raw->children[1]->data);
  } else {
    cout << "Erro: servidor sem ip ou usuário." << endl;
  }
}
