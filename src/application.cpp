#include <fstream>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <iomanip>
#include <iostream>

#include "en_de.h"

std::string current_date_time_string(const bool includeMilliseconds = true) {
  // 获取当前时间点
  const auto now = std::chrono::system_clock::now();

  // 转换为 time_t 类型
  const std::time_t now_c = std::chrono::system_clock::to_time_t(now);

  // 转换为 tm 结构
  const std::tm now_tm = *std::localtime(&now_c);

  // 提取年月日时分秒
  const int year   = now_tm.tm_year + 1900;
  const int month  = now_tm.tm_mon + 1;
  const int day    = now_tm.tm_mday;
  const int hour   = now_tm.tm_hour;
  const int minute = now_tm.tm_min;
  const int second = now_tm.tm_sec;

  // 构建时间字符串
  std::stringstream ss;
  ss << year << "-" << std::setw(2) << std::setfill('0') << month << "-" << std::setw(2) << std::setfill('0') << day
     << " " << std::setw(2) << std::setfill('0') << hour << ":" << std::setw(2) << std::setfill('0') << minute << ":"
     << std::setw(2) << std::setfill('0') << second;

  if (includeMilliseconds) {
    // 获取毫秒数
    const auto now_ms      = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    const int milliseconds = now_ms.time_since_epoch().count() % 1000;

    ss << "." << std::setw(3) << std::setfill('0') << milliseconds;
  }

  return std::string("[" + ss.str() + "]\n");
}

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

      std::string filepath;
      std::string filename;
      std::string password;
      int operation = 0;  // 0 for encrypt, 1 for decrypt
      std::string status_message{current_date_time_string() + "Welcome to FileEncryptor!"};

      std::vector<std::string> options = {"Encrypt", "Decrypt"};

      auto input_filepath = Input(&filepath, "filepath");
      auto input_filename = Input(&filename, "filename");
      auto input_password = Input(&password, "password");
      auto radiobox       = Radiobox(&options, &operation);

      auto screen = ScreenInteractive::TerminalOutput();

      auto button_exec = Button("Execute", [&] {
        if (operation == 0) {
          try {
            encrypt_file(filepath + filename, password);
            status_message = current_date_time_string() + "encryptFile success:" + filename + ".enc";
          } catch (...) {
            status_message = current_date_time_string() + "encryptFile failed.";
          }
        } else {
          try {
            decrypt_file(filepath + filename, password);
            status_message = current_date_time_string() + "decryptFile success:" + filename + ".de";
          } catch (...) {
            status_message = current_date_time_string() + "decryptFile failed.";
          }
        }
      });
      auto button_exit = Button("Exit", screen.ExitLoopClosure());

      auto renderer = Renderer(Container::Vertical({input_filepath, input_filename, input_password, radiobox,
                                                    Container::Horizontal({button_exec, button_exit})}),
                               [&] {
                                 return vbox({
                                            text("File Encryption/Decryption") | bold | center,
                                            separator(),
                                            hbox(text("File Path: "), input_filepath->Render()),
                                            hbox(text("File Name: "), input_filename->Render()),
                                            hbox(text("Password : "), input_password->Render()),
                                            hbox(text("Operation: "), radiobox->Render()),
                                            hbox(button_exec->Render() | center, button_exit->Render() | center),
                                            separator(),
                                            text(status_message) | center,
                                        }) |
                                        border | size(WIDTH, EQUAL, 60) | size(HEIGHT, EQUAL, 15);
                               });

      screen.Loop(renderer);
    }  // case ui::FTXUI
    default: {
      break;
    }  // default
  }    // switch

  return 0;
}
