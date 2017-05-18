#include <nanogui/nanogui.h>
#include <iostream>
#include <cstdlib>
#include "network.h"

using namespace nanogui;

// Used in the user verification dialog
std::string username = "administrador";
std::string password = "";

Screen *screen;

namespace window {
  Window* sections(FormHelper*, Network*);
}

namespace generate_screen {
  void main (FormHelper *gui, Network* net) {
    Window* sections = window::sections(gui, net);

    screen->performLayout();
  }
}

namespace window {
  Window* login (FormHelper *gui, string ip) {
    Window* login =
    new Window(screen, "Login em "+ip);

    AdvancedGridLayout *layout =
    dynamic_cast<AdvancedGridLayout*>(login->layout());

    Label *label = new Label(login, "");
    layout->appendRow(0);
    layout->setAnchor(label, AdvancedGridLayout::Anchor(0, 0, 4, 1, Alignment::Middle, Alignment::Middle));

    label->setColor(Color(200, 0, 0, 255));

    gui->addGroup("");

    gui->addVariable("Usuário", username);
    gui->addVariable("Senha", password, true, true);

    gui->addGroup("");

    return login;
  }

  Window* section (FormHelper *gui, Section *secDesc) {
    Window* section =
    new Window(screen, secDesc->name);

    section->setPosition(Vector2i(270, 10));
    section->setSize(Vector2i(200, 200));
    section->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 5, 10));

    VScrollPanel *vscroll = new VScrollPanel(section);
    Widget *list = new Widget(vscroll);
    list->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 5, 10));

    vscroll->setFixedHeight(200);

    for (unsigned int i=0;i<secDesc->hosts.size();i++) {
      Widget *spacer;
      std::string ip = secDesc->hosts[i]->ip.asString();
      std::string user = secDesc->hosts[i]->user;
      std::string hostname = secDesc->hosts[i]->hostname;

      auto row = new Widget(list);
      row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Fill));

      auto accessButton =
      new Button(row, "SSH", ENTYPO_ICON_PUBLISH);

      accessButton->setCallback([accessButton, gui, ip] {
        Window* login = window::login();

        std::system(("x-terminal-emulator -e /bin/bash -c 'ssh -XC administrador@"+ip+"; read'").c_str());

        //Window* section = window::section(gui, title);

        //openButton->setChangeCallback([section] (bool on) {
        //  if (!on) {
        //    section->dispose();
        //  }
        //});

        screen->performLayout();
      });

      spacer = new Widget(row);
      spacer->setFixedWidth(6);

      auto graphicalAccessButton =
      new Button(row, "VNC", ENTYPO_ICON_PUBLISH);

      graphicalAccessButton->setCallback([graphicalAccessButton, gui, ip] {
        std::system(("x-terminal-emulator -e \"/bin/bash -c 'ssh administrador@"+ip+"; read'\"").c_str());

        //Window* section = window::section(gui, title);

        //openButton->setChangeCallback([section] (bool on) {
        //  if (!on) {
        //    section->dispose();
        //  }
        //});

        screen->performLayout();
      });

      spacer = new Widget(row);
      spacer->setFixedWidth(6);

      //auto infoButton =
      //new Button(row, "", ENTYPO_ICON_INFO);

      //infoButton->setFlags(Button::ToggleButton);


      new Label(row, ip);

      spacer = new Widget(row);
      spacer->setFixedWidth(6);

      new Label(row, hostname);

      spacer = new Widget(row);
      spacer->setFixedWidth(6);

      new Label(row, user);

      spacer = new Widget(row);
      spacer->setFixedWidth(6);
    }

    return section;
  }

  Window* sections (FormHelper *gui, Network *net) {
    Window* sections =
    new Window(screen, "SEÇÕES");

    sections->setPosition(Vector2i(10, 10));
    sections->setSize(Vector2i(200, 200));
    sections->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 5, 10));

    VScrollPanel *vscroll = new VScrollPanel(sections);
    Widget *list = new Widget(vscroll);
    list->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 5, 10));

    vscroll->setFixedHeight(300);

    for (unsigned int i=0;i<net->sections.size();i++) {
      Widget *spacer;
      std::string title = net->sections[i]->name;

      auto row = new Widget(list);
      row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Fill));

      auto openButton =
      new Button(row, "Acessar", ENTYPO_ICON_ARCHIVE);

      openButton->setFlags(Button::ToggleButton);

      auto sectionDescription = net->sections[i];

      openButton->setCallback([openButton, gui, sectionDescription] {
        //std::system("x-terminal-emulator -e \"bash -c 'ssh administrador@localhost; read'\"");

        Window* section = window::section(gui, sectionDescription);

        openButton->setChangeCallback([section] (bool on) {
          if (!on) {
            section->dispose();
          }
        });

        screen->performLayout();
     });

      spacer = new Widget(row);
      spacer->setFixedWidth(6);

      auto infoButton =
      new Button(row, "", ENTYPO_ICON_INFO);

      infoButton->setFlags(Button::ToggleButton);

      spacer = new Widget(row);
      spacer->setFixedWidth(6);

      new Label(row, title);
    }

    return sections;
  }

  Window* blocked (FormHelper *gui, Network *net) {
    Window* blocked =
    gui->addWindow(Eigen::Vector2i(0, 0), "NetMon 1° BEC");

    AdvancedGridLayout *layout =
    dynamic_cast<AdvancedGridLayout*>(blocked->layout());

    Label *label = new Label(blocked, "");
    layout->appendRow(0);
    layout->setAnchor(label, AdvancedGridLayout::Anchor(0, 0, 4, 1, Alignment::Middle, Alignment::Middle));

    label->setColor(Color(200, 0, 0, 255));

    gui->addGroup("");

    gui->addVariable("Usuário", username);
    gui->addVariable("Senha", password, true, true);

    gui->addGroup("");

    gui->addButton("Desbloquear", [label, blocked, gui, net]() {
      if (username == "administrador" && password == "AsitotnB") {
        // Start App
        blocked->dispose();

        generate_screen::main(gui, net);
      } else {
        // Show error
        label->setCaption("Usuário/Senha inválidos");
        screen->performLayout();
      }
    });

    return blocked;
  }
}

int main(int, char **) {
  cout << "NetMon v0.1" << endl;

  // Read configuration file
  auto conf = new Configuration("1bec.conf");
  conf->read();
  // Create a network description
  // from the configuration
  auto net = new Network(conf);

  nanogui::init();

  {
    screen =
    new Screen(Vector2i(800, 600), "NetMon", false, false /* fullscreen */);

    bool enabled = true;
    FormHelper *gui = new FormHelper(screen);
    Window* blocked = window::blocked(gui, net);

    screen->setVisible(true);
    screen->performLayout();

    blocked->center();

    nanogui::mainloop();
  }

  // Write any changes in configuration
  // TODO: DO THIS AT THE TIME OF THE CHANGE
  conf->write();

  nanogui::shutdown();

  delete net;
  delete conf;

  return 0;
}
