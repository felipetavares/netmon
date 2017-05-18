#include <iostream>
#include <string>
#include <sstream>
#include "network.h"

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

// Section

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

// Host

Host::Host (Element *raw) {
  if (raw->children.size() != 2) {
    cout << "Erro: número de entradas em host inválido." << endl;
  } else {
    hostname = raw->data;

    user = raw->children[0]->data;
    ip = IP(raw->children[1]->data);
  }
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

char* IP::asBytes() {
  return ip;
}

// Server

Server::Server (Element* raw) {
  name = raw->data;

  if (raw->children.size() == 1) {
    ip = IP(raw->children[0]->data);
  } else {
    cout << "Erro: servidor sem ip." << endl;
  }
}
