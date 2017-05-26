#ifndef NETMON_UPDATE_H
#define NETMON_UPDATE_H

#include <functional>
#include <map>

using namespace std;

namespace update {
  void remove(unsigned long);
  unsigned long add(function<void()>);
  void run();
}

#endif
