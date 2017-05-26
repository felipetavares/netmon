#include <nanogui/nanogui.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "network.h"
#include "update.h"

#define UP_STR string(utf8(ENTYPO_ICON_UP).data())
#define DOWN_STR string(utf8(ENTYPO_ICON_DOWN).data())

using namespace nanogui;

// Used in the user verification dialog
std::string username = "administrador";
std::string password = "";

Screen *screen;

namespace window {
  Window* sections(FormHelper*, Network*);
  Window* servers(FormHelper*, Network*);
  Window* update_sections(FormHelper*, Network*, Window*);
}

namespace generate_screen {
  void main (FormHelper *gui, Network* net) {
    Window* sections = window::sections(gui, net);
    Window* servers = window::servers(gui, net);

    screen->performLayout();
  }
}

namespace window {
  Window* newhost (FormHelper *gui) {
    Window* newhost =
    gui->addWindow(Eigen::Vector2i(0, 0), "Adicionar máquina");

    std::string *hostname = new string("Nome da máquina");
    std::string *username = new string("");
    std::string *ip = new string("10.46.72.1");
    std::string *mac = new string("00:00:00:00:00:00");

    gui->addGroup("Geral");

    gui->addVariable("Hostname", *hostname);
    gui->addVariable("Usuário", *username);

    gui->addGroup("Endereço");

    gui->addVariable("IP", *ip);
    gui->addVariable("MAC", *mac);

    gui->addGroup("");

    gui->addButton("Cancelar", []() {

    });

    gui->addButton("Criar", []() {

    });

    newhost->center();

    return newhost;
  }

  Window* newsection (FormHelper *gui, Network* net, Window* sections) {
    Window* newsection =
    gui->addWindow(Eigen::Vector2i(0, 0), "Adicionar seção");

    std::string *name = new string("Nome da seção");

    gui->addGroup("Geral");

    gui->addVariable("Nome", *name);

    gui->addGroup("");

    gui->addButton("Cancelar", [newsection, name]() {
      newsection->dispose();
      delete name;
    });

    gui->addButton("Criar", [newsection, name, gui, net, sections]() {
      net->sections.push_back(new Section(*name));

      window::update_sections(gui, net, sections);

      newsection->dispose();
      delete name;
    });

    newsection->center();

    return newsection;
  }

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

    gui->addButton("Cancelar", [login, username, program, graphicAccess]() {
      login->dispose();
      delete username;
      delete program;
      delete graphicAccess;
    });

    gui->addButton("Logar", [login, ip, username, program, graphicAccess]() {
      login->dispose();

      std::string arguments = (*graphicAccess)?"-XC":"";

      std::system(
        (
          "xterm -e "+
          (*program)+
          " -c 'ssh "+
          arguments+
          " "+
          (*username)+"@"+ip+
          "; read' &")
        .c_str()
      );

      delete username;
      delete program;
      delete graphicAccess;
    });

    return login;
  }

  Window* vnclogin (FormHelper *gui, string ip) {
    Window* login =
    gui->addWindow(Eigen::Vector2i(0, 0), "Login em "+ip);

    std::string *password = new string("AsitotnB");
    bool *fullscreen = new bool(false);
    bool *viewonly = new bool(true);

    AdvancedGridLayout *layout =
    dynamic_cast<AdvancedGridLayout*>(login->layout());

    gui->addGroup("");

    gui->addVariable("Senha", *password, true, true);

    gui->addGroup("");

    gui->addVariable("Fullscreen", *fullscreen);
    gui->addVariable("Apenas visualizar", *viewonly);

    gui->addGroup("");

    gui->addButton("Cancelar", [login, password, fullscreen, viewonly]() {
      login->dispose();
      delete password;
      delete fullscreen;
      delete viewonly;
    });

    gui->addButton("Logar", [login, ip, password, fullscreen, viewonly]() {
      login->dispose();

      std::system(
        (
          "echo '"+*password+
          "' | vncviewer -compresslevel 5 -quality 5 -autopass"+
          (*viewonly?" -viewonly":"")+
          (*fullscreen?" -fullscreen":"")+
          " "+ip+" &"
        ).c_str()
      );

      delete password;
      delete fullscreen;
      delete viewonly;
    });

    return login;
  }

  Window* servers (FormHelper *gui, Network *net) {
    Window* servers =
    new Window(screen, "SERVIDORES");

    servers->setPosition(Vector2i(20, 20));
    servers->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 10));

    VScrollPanel *vscroll = new VScrollPanel(servers);
    Widget *list = new Widget(vscroll);
    list->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 5, 10));

    vscroll->setFixedHeight(300);

    for (unsigned int i=0;i<net->servers.size();i++) {
      Widget *spacer;
      std::string title = net->servers[i]->name;
      std::string ip = net->servers[i]->ip.asString();
      auto row = new Widget(list);
      row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Fill));

      auto openButton =
      new Button(row, "SSH", ENTYPO_ICON_PUBLISH);

      auto serverDescription = net->servers[i];

      openButton->setCallback([openButton, gui, serverDescription, ip] {
        std::system(
          (
            string("xterm -e ")+
            string("bash")+ // shell
            string(" -c 'ssh ")+
            string("")+ // arguments
            string(" ")+
            (serverDescription->username)+"@"+ip+
            string("; read' &"))
          .c_str()
        );
      });

      spacer = new Widget(row);
      spacer->setFixedWidth(6);

      new Label(row, title);
    }

    return servers;
  }

  Window* section (FormHelper *gui, Section *secDesc) {
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
      auto accessVncButton =
      new Button(list, "VNC", ENTYPO_ICON_PUBLISH);
      layout->setAnchor(accessVncButton, AdvancedGridLayout::Anchor(2, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      accessVncButton->setCallback([accessButton, gui, ip, up] {
        Window* login = window::vnclogin(gui, ip);

        screen->performLayout();

        login->center();
      });

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(3, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto ipLabel = new Label(list, ip);
      layout->setAnchor(ipLabel, AdvancedGridLayout::Anchor(4, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(5, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto hostnameLabel = new Label(list, hostname);
      layout->setAnchor(hostnameLabel, AdvancedGridLayout::Anchor(6, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(7, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto userLabel = new Label(list, user);
      layout->setAnchor(userLabel, AdvancedGridLayout::Anchor(8, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(9, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendCol(0);
      auto pingLabel = new Label(list, up?UP_STR:DOWN_STR, "icons");
      pingLabel->setFontSize(28);
      layout->setAnchor(pingLabel, AdvancedGridLayout::Anchor(10, i*2, 1, 1, Alignment::Minimum, Alignment::Middle));

      if (up) {
        pingLabel->setColor(Color(0, 200, 0, 255));
      } else {
        pingLabel->setColor(Color(200, 0, 0, 255));
      }

      Host *host = secDesc->hosts[i];

      unsigned long updater = update::add([pingLabel, host]() {
        pingLabel->setCaption(host->up?UP_STR:DOWN_STR);

        if (host->up) {
          pingLabel->setColor(Color(0, 200, 0, 255));
        } else {
          pingLabel->setColor(Color(200, 0, 0, 255));
        }

        //window->performLayout();
      });

      secDesc->addUpdater(updater);

      layout->appendCol(0);
      spacer = new Widget(list);
      spacer->setFixedWidth(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(11, i*2, 1, 1, Alignment::Middle, Alignment::Middle));

      layout->appendRow(0);
      spacer = new Widget(list);
      spacer->setFixedHeight(6);
      layout->setAnchor(spacer, AdvancedGridLayout::Anchor(0, i*2+1, 4, 1, Alignment::Middle, Alignment::Middle));
    }

    section->center();

    return section;
  }

  Window* update_sections (FormHelper *gui, Network *net, Window* sections) {
    // TODO: update the sections window with new buttons
  }

  Window* sections (FormHelper *gui, Network *net) {
    Window* sections =
    new Window(screen, "SEÇÕES");

    sections->setPosition(Vector2i(0, 0));
    sections->setSize(Vector2i(200, 200));
    sections->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 5, 10));

    auto addButton =
    new Button(sections, "", ENTYPO_ICON_PLUS);

    addButton->setCallback([gui, net, sections]() {
      Window *newSection = window::newsection(gui, net, sections);
      screen->performLayout();
    });

    VScrollPanel *vscroll = new VScrollPanel(sections);
    Widget *list = new Widget(vscroll);
    list->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 0, 0));

    vscroll->setFixedHeight(600);

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

    return sections;
  }

  Window* blocked (FormHelper *gui, Network *net) {
    Window* blocked =
    gui->addWindow(Eigen::Vector2i(0, 0), "Login NetMon");

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

void updateInterface() {
  static unsigned int runTime = 0;

  // Run every frame
  if (runTime++ % 1 == 0) {
    update::run();
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

      std::this_thread::sleep_for(std::chrono::seconds(10));
    }
  });

  pinger.detach();

  custom(updateInterface);

  nanogui::init();

  {
    screen =
    new Screen(Vector2i(800, 600), "Rede "+net->name, true, false /* fullscreen */);

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
