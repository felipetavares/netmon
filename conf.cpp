#include "conf.h"
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

Configuration::Configuration (string filename):
  filename(filename),
  file(filename.c_str(), fstream::in | fstream::out) {  
}

Configuration::~Configuration () {
  if (root != NULL) {
    deleteChildren(root);
    root = NULL;
  }
}

void Configuration::debugElement(Element *e) {
  cout << e->level << " " << e->data << " {" << endl;

  for (auto c :e->children) {
    debugElement(c);
  }

  cout << "}" << endl;
}

Element* Configuration::getRoot() {
  return root;
}

void Configuration::debug () {
  debugElement(root);
}

void Configuration::write () {
  file.seekp(0);

  if (root != NULL) {
    writeElement(root);
  }
}

string Configuration::generateSpaces (unsigned int n) {
  string spaces;

  for (unsigned int i=0;i<n;i++) {
    spaces += ' ';
  }

  return spaces;
}

void Configuration::writeElement (Element *e) {
  // Real deal
  //file << generateSpaces(e->level) << e->data << endl;
  // Debug
  cout << generateSpaces(e->level) << e->data << endl;

  for (auto child :e->children) {
    writeElement(child);
  }
}

void Configuration::read () {
  string line;
  stack<Element*> estack;

  while (!file.eof()) {
    getline(file, line);

    Element *e = new Element();
    e->data = dataOf(line);
    e->level = levelOf(line);

    if (e->data == "") {
      delete e;
      continue;
    }
    
    if (!estack.empty()) {
      if (estack.top()->level >= e->level) {
	while (estack.top()->level >= e->level &&
	       estack.size() > 1) {
	  estack.pop();
	}
      }
      estack.top()->children.push_back(e);
    } else {
      root = e;
    }
  
    estack.push(e);
  }
}

void Configuration::deleteChildren(Element *e) {
  for (auto i :e->children) {
    deleteChildren(i);
  }

  delete e;
}

// Remove all whitespace from the beginning
// of the line and return it
string Configuration::dataOf(string& line) {
  unsigned int index = levelOf(line);
  
  // Return a substring startig at the found index
  return line.substr(index);
}

unsigned int Configuration::levelOf(string& line) {
  unsigned int lineSize = line.size();
  unsigned int index = 0;
  
  // Find the index of the first non-whitespace character
  for (;index<lineSize;index++) {
    if (line[index] != ' ') {
      break;
    }
  }

  return index;
}

int main () {
  cout << "NetMon Configuration File Reader" << endl;

  auto conf = new Configuration("1bec.conf");

  conf->read();
  conf->write();
  
  delete conf;
  
  return 0;
}
