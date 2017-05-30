#include "gui.h"
#include "main.h"

SectionsWindow::SectionsWindow (FormHelper *gui, Network *net) {
  // Create window
  Window* window =
  new Window(screen, "SEÇÕES");

  // Set initial layout and position
  window->setPosition(Vector2i(0, 0));
  window->setSize(Vector2i(200, 200));
  window->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 10));

  // Add section button
  auto addButton =
  new Button(window, "", ENTYPO_ICON_PLUS);

  addButton->setCallback([gui, net, window, this]() {
    Window *newSection = window::newsection(gui, net, window, this);
    screen->performLayout();
  });

  // Scrollable panel
  VScrollPanel *vscroll = new VScrollPanel(window);
  list = new Widget(vscroll);
  list->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, 0));

  vscroll->setFixedHeight(600);

  // Sections
  for (unsigned int i=0;i<net->sections.size();i++) {
    addSection(gui, net->sections[i]);
  }
}


void SectionsWindow::addSection (FormHelper *gui, Section *sectionDescription) {
  Widget *spacer;
  string title = sectionDescription->name;

  auto row = new Widget(list);
  row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Fill));

  auto openButton =
  new Button(row, "Acessar", ENTYPO_ICON_ARCHIVE);

  openButton->setFlags(Button::ToggleButton);

  openButton->setCallback([openButton, gui, sectionDescription] {
    Window* section = window::section(gui, sectionDescription);

    openButton->setChangeCallback([section, sectionDescription] (bool on) {
      if (!on) {
        sectionDescription->removeUpdaters();
        section->dispose();
      }
    });

    screen->performLayout();
  });

  spacer = new Widget(row);
  spacer->setFixedWidth(6);

  new Label(row, title);

  spacer = new Widget(list);
  spacer->setFixedHeight(6);
}
