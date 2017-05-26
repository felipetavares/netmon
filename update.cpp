#include "update.h"

#define MAX_UPDATES_PER_FRAME 3

namespace update {
  typedef map<unsigned long, function<void()>> fnmap;
  typedef pair<unsigned long, function<void()>> fnpair;

  unsigned long key = 0;
  fnmap hooks;
}

unsigned long update::add (function<void()> hook) {
  unsigned long currentKey = key++;

  hooks.insert(fnpair(currentKey, hook));

  return currentKey;
}

void update::remove(unsigned long p) {
  hooks.erase(p);
}

void update::run () {
  static fnmap::iterator i = hooks.end();
  unsigned int j;

  if (i == hooks.end()) {
    i = hooks.begin();
    return;
  }

  for (j=0;j<MAX_UPDATES_PER_FRAME;j++) {
    i->second();

    if (++i == hooks.end()) {
      break;
    }
  }
}
