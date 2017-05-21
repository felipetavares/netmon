#include <nanogui/nanogui.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
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
    gui->addWindow(Eigen::Vector2i(0, 0), "Login em "+ip);

    std::string *username = new string("administrador");
    std::string *program = new string("bash");
    bool *graphicAccess = new bool(false);

    AdvancedGridLayout *layout =
    dynamic_cast<AdvancedGridLayout*>(login->layout());

    gui->addGroup("");

    gui->addVariable("Usuário", *username);
    gui->addVariable("Programa", *program);

    gui->addGroup("");

    gui->addVariable("X11", *graphicAccess);

    gui->addGroup("");

    gui->addButton("Logar", [login, ip, username, program, graphicAccess]() {
      std::string arguments = (*graphicAccess)?"-XC":"";

      std::system(
        (
          "xterm -e "+
          (*program)+
          " -c 'ssh "+
          arguments+
          " "+
          (*username)+"@"+ip+
          "; read'")
        .c_str()
      );

      delete username;
      delete program;
      delete graphicAccess;

      login->dispose();
    });

    return login;
  }

  Window* section (FormHelper *gui, Section *secDesc) {
    cout << "] " << secDesc->up() << endl;

    Window* section =
    new Window(screen, secDesc->name);

    section->setPosition(Vector2i(270, 10));
    section->setSize(Vector2i(200, 200));
    section->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 10));

    new Label(section, "Computadores Ativos");
    auto hostsUp = new ProgressBar(section);

    hostsUp->setValue((float)secDesc->up()/(float)secDesc->hosts.size());

    VScrollPanel *vscroll = new VScrollPanel(section);
    Widget *list = new Widget(vscroll);
    auto layout = new AdvancedGridLayout({}, {}, 0);
    list->setLayout(layout);

    vscroll->setFixedHeight(200);

    for (unsigned int i=0;i<secDesc->hosts.size();i++) {
      Widget *spacer;
      std::string ip = secDesc->hosts[i]->ip.asString();
      std::string user = secDesc->hosts[i]->user;
      std::string hostname = secDesc->hosts[i]->hostname;
      bool up = secDesc->hosts[i]->up;

      layout->appendRow(0);
      layout->appendCol(0);

      auto accessButton =
      new Button(list, "SSH", ENTYPO_ICON_PUBLISH);
      layout->setAnchor(accessButton, AdvancedGridLayout::Anchor(0, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      accessButton->setCallback([accessButton, gui, ip, up] {
        Window* login = window::login(gui, ip);

        screen->performLayout();

        login->center();
      });

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(1, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto ipLabel = new Label(list, ip);
      layout->setAnchor(ipLabel, AdvancedGridLayout::Anchor(2, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(3, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto hostnameLabel = new Label(list, hostname);
      layout->setAnchor(hostnameLabel, AdvancedGridLayout::Anchor(4, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(5, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto userLabel = new Label(list, user);
      layout->setAnchor(userLabel, AdvancedGridLayout::Anchor(6, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(7, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto pingLabel = new Label(list, up?"UP":"DOWN");
      layout->setAnchor(pingLabel, AdvancedGridLayout::Anchor(8, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      if (up) {
        pingLabel->setColor(Color(0, 200, 0, 255));
      } else {
        pingLabel->setColor(Color(200, 0, 0, 255));
      }

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(9, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendRow(0);
      spacer = new Widget(list);
      spacer->setFixedHeight(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(0, i*2+1, 4, 1, Alignment::Middle, Alignment::Middle));
    }

    section->center();

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

  thread pinger([net]() {
    while (true) {
      for (auto s :net->sections) {
        for (auto h :s->hosts) {
          h->ping();
        }
      }

      std::this_thread::sleep_for(std::chrono::seconds(30));
    }
  });

  pinger.detach();

  nanogui::init();

  {
    screen =
    new Screen(Vector2i(800, 600), "NetMon", true, false /* fullscreen */);

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
