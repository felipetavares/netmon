#include <nanogui/nanogui.h>
#include <iostream>
#include <cstdlib>

using namespace nanogui;

// Used in the user verification dialog
std::string username = "administrador";
std::string password = "";

Screen *screen;

namespace window {
  Window* sections(FormHelper*);
}

namespace generate_screen {
  void main (FormHelper *gui) {
    Window* sections = window::sections(gui);
    
    screen->performLayout();
  }
}

namespace window {
  Window* section (FormHelper *gui, std::string secName) {
    Window* section =
    new Window(screen, "SEC " + secName);

    section->setPosition(Vector2i(250, 0));
    section->setSize(Vector2i(200, 200));
    section->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 5, 10));

    VScrollPanel *vscroll = new VScrollPanel(section);
    Widget *list = new Widget(vscroll);
    list->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 5, 10));

    vscroll->setFixedHeight(200);

    for (unsigned int i=0;i<20;i++) {
      Widget *spacer;
      std::string ip = "10.56.72.";

      for (unsigned int j=0;j<3;j++)
        ip += char(rand()%3+'0');

      auto row = new Widget(list);
      row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Fill));

      auto accessButton =
      new Button(row, "Acesso Remoto", ENTYPO_ICON_PUBLISH);
      
      //openButton->setFlags(Button::ToggleButton);

      //openButton->setCallback([openButton, gui, title] {
        //std::system("x-terminal-emulator -e \"bash -c 'ssh administrador@localhost; read'\"");
        
        //Window* section = window::section(gui, title);

        //openButton->setChangeCallback([section] (bool on) {
        //  if (!on) {
        //    section->dispose();
        //  }
        //});

        //screen->performLayout();
     //});

      //spacer = new Widget(row);
      //spacer->setFixedWidth(6);

      //auto infoButton =
      //new Button(row, "", ENTYPO_ICON_INFO);

      //infoButton->setFlags(Button::ToggleButton);

      //spacer = new Widget(row);
      //spacer->setFixedWidth(6);

      new Label(row, ip);
    }

    return section;
  }

  Window* sections (FormHelper *gui) {
    std::string sectionNames[] = {
      "OP PIPA",
      "SIP",
      "S1",
      "S2",
      "S3",
      "S4"
    };

    Window* sections =
    new Window(screen, "SEÇÕES");
    
    sections->setPosition(Vector2i(10, 10));
    sections->setSize(Vector2i(200, 200));
    sections->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 5, 10));

    VScrollPanel *vscroll = new VScrollPanel(sections);
    Widget *list = new Widget(vscroll);
    list->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 5, 10));

    vscroll->setFixedHeight(300);

    for (unsigned int i=0;i<6;i++) {
      Widget *spacer;      
      std::string title = sectionNames[i];

      auto row = new Widget(list);
      row->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Fill));

      auto openButton =
      new Button(row, "Acessar", ENTYPO_ICON_ARCHIVE);
      
      openButton->setFlags(Button::ToggleButton);

      openButton->setCallback([openButton, gui, title] {
        //std::system("x-terminal-emulator -e \"bash -c 'ssh administrador@localhost; read'\"");
        
        Window* section = window::section(gui, title);

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

  Window* blocked (FormHelper *gui) {
    Window* blocked =
    gui->addWindow(Eigen::Vector2i(0, 0), "Bloqueado");
    
    AdvancedGridLayout *layout =
    dynamic_cast<AdvancedGridLayout*>(blocked->layout());

    Label *label = new Label(blocked, "");
    layout->appendRow(0);
    layout->setAnchor(label, AdvancedGridLayout::Anchor(0, 0, 4, 1, Alignment::Middle, Alignment::Middle));

    label->setColor(Color(200, 0, 0, 255));

    gui->addGroup("");

    gui->addVariable("Usuário", username);
    gui->addVariable("Senha", password);

    gui->addGroup("");

    gui->addButton("Desbloquear", [label, blocked, gui]() {
      if (username == "administrador" && password == "AsitotnB") {
        // Start App
        blocked->dispose();

        generate_screen::main(gui);
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
    nanogui::init();

    {
        screen =
        new Screen(Vector2i(800, 600), "NetMon", false, false /* fullscreen */);

        bool enabled = true;
        FormHelper *gui = new FormHelper(screen);
        Window* blocked = window::blocked(gui);
        
        screen->setVisible(true);
        screen->performLayout();
        
        blocked->center();

        nanogui::mainloop();
    }

    nanogui::shutdown();
    return 0;
}
