#include "network.h"

void Network::fillData(Configuration *conf) {
  Element *root = conf->getRoot();
  Network *net = new Network();
  
  if (root != NULL) {
    network->name = root->data;

    // Servers
    Element *servers = getServer(root);

    if (servers != NULL) {
      
    } else {
      // ERRO. SEM SEÇÃO DE SERVIDORES
    }
    // Sections
    Element *sections = getSections(root);

    if (servers != NULL) {
      
    } else {
      // ERRO. SEM SEÇÃO DE SEÇÕES
    }
  } else {
    // ERRO. DADOS INVÁLIDOS
  }
}
