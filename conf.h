#ifndef NETMON_CONF_H
#define NETMON_CONF_H

#include <fstream>
#include <vector>
using namespace std;

typedef struct Element {
  unsigned int level;
  string data;
  vector <Element*> children;
}Element;

class Configuration {
  string filename;
  fstream file;
  Element *root = NULL;
public:
  Configuration(string);
  ~Configuration();
  void read();
  void write();
  void debug();
  Element* getRoot();
private:
  void deleteChildren(Element*);
  string dataOf(string&);
  unsigned int levelOf(string&);
  void debugElement(Element*);
  void writeElement(Element*);
  string generateSpaces(unsigned int);
};

#endif
