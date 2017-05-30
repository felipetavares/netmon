#ifndef NETMON_MAIN_H
#define NETMON_MAIN_H

#include <nanogui/nanogui.h>
#include "network.h"
#include "gui.h"

using namespace nanogui;

namespace window {
  Window* sections(FormHelper*, Network*);
  Window* servers(FormHelper*, Network*);
  Window* update_sections(FormHelper*, Network*, Window*);
  Window* newsection(FormHelper*, Network*, Window*, SectionsWindow*);
  Window* section(FormHelper*, Section*);
}

extern Screen *screen;

#endif
