#ifndef NETMON_GUI_H
#define NETMON_GUI_H

#include <nanogui/nanogui.h>
#include "network.h"

using namespace nanogui;

class SectionsWindow {
 public:
  Window *window;
  Widget *list;

  SectionsWindow(FormHelper*, Network*);
  void addSection(FormHelper*, Section*);
};

#endif
