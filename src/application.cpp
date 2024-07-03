#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iostream>

#include "en_de.h"

int main(int argc, char *argv[]) {
  enum class ui : int { CONSOLE, FTXUI };

  switch (constexpr auto use = ui::FTXUI; use) {
    case ui::CONSOLE: {
      if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <filename> <password> <en|de>" << std::endl;
        return 1;
      }

      std::string filename  = argv[1];
      std::string password  = argv[2];
      std::string operation = argv[3];

      if (operation == "en") {
        encrypt_file(filename, password);
      } else if (operation == "de") {
        decrypt_file(filename, password);
      } else {
        std::cerr << "Invalid operation: " << operation << std::endl;
        return 1;
      }
      break;
    }  // case ui::CONSOLE
    case ui::FTXUI: {
      using namespace ftxui;

      std::string filename;
      std::string password;
      int operation = 0;  // 0 for encrypt, 1 for decrypt
      std::string status_message{"Welcome to the file encryption and decryption system!"};

      std::vector<std::string> options = {"Encrypt", "Decrypt"};

      auto input_filename = Input(&filename, "filename");
      auto input_password = Input(&password, "password");
      auto radiobox       = Radiobox(&options, &operation);

      auto screen = ScreenInteractive::TerminalOutput();

      auto button_exec = Button("Execute", [&] {
        if (operation == 0) {
          encrypt_file(filename, password);
          status_message = "encryptFile success:" + filename + ".enc";
        } else {
          decrypt_file(filename, password);
          status_message = "decryptFile success:" + filename + ".de";
        }
      });
      auto button_exit = Button("Exit", screen.ExitLoopClosure());

      auto renderer = Renderer(Container::Vertical({input_filename, input_password, radiobox,
                                                    Container::Horizontal({button_exec, button_exit})}),
                               [&] {
                                 return vbox({
                                            text("File Encryption/Decryption") | bold | center,
                                            separator(),
                                            hbox(text("Filename: "), input_filename->Render()),
                                            hbox(text("Password: "), input_password->Render()),
                                            hbox(text("Operation: "), radiobox->Render()),
                                            hbox(button_exec->Render() | center, button_exit->Render() | center),
                                            separator(),
                                            text(status_message) | center,
                                        }) |
                                        border | size(WIDTH, EQUAL, 50) | size(HEIGHT, EQUAL, 15);
                               });

      screen.Loop(renderer);
    }  // case ui::FTXUI
    default: {
      break;
    }  // default
  }    // switch

  return 0;
}
