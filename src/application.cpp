#include <iostream>

#include "encryption.h"
#include "json_handler.h"

void displayMenu() {
  std::cout << "Menu:\n";
  std::cout << "1. Add data\n";
  std::cout << "2. Get data\n";
  std::cout << "3. Delete data\n";
  std::cout << "4. Exit\n";
  std::cout << "Enter your choice: ";
}

int main(int argc, char* argv[]) {
  auto _ = encrypt("shecannotsee","123");
  std::cout <<_<< "\n"<< decrypt(_,"123")<< std::endl;
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <json_file> <password>" << std::endl;
    return 1;
  }

  std::string jsonFilePath = argv[1];
  std::string password     = argv[2];

  JsonHandler jsonHandler(jsonFilePath, password);
  if (!jsonHandler.loadJson()) {
    std::cerr << "Failed to load JSON file or invalid password." << std::endl;
    return 1;
  }

  int choice;
  enum CHOICE : int { ADD_DATA = 1, GET_DATA = 2, DELETE_DATA = 3, EXIT = 4 };
  while (true) {
    displayMenu();
    std::cin >> choice;

    if (choice == static_cast<int>(ADD_DATA)) {
      std::string parentKey, childKey, value, encrypt;
      std::cout << "Enter parent key: ";
      std::cin >> parentKey;
      std::cout << "Enter child key: ";
      std::cin >> childKey;
      std::cout << "Enter value: ";
      std::cin >> value;
      std::cout << "Encrypt value? (yes/no): ";
      std::cin >> encrypt;
      jsonHandler.addData(parentKey, childKey, value, encrypt == "yes");
    } else if (choice == static_cast<int>(GET_DATA)) {
      std::string parentKey, childKey, encrypt;
      std::cout << "Enter parent key: ";
      std::cin >> parentKey;
      std::cout << "Enter parent child key: ";
      std::cin >> childKey;
      std::cout << "Encrypt value? (yes/no): ";
      std::cin >> encrypt;
      nlohmann::json data = jsonHandler.getData(parentKey, childKey, encrypt == "yes");
      std::cout << "Data: " << data.dump(4) << std::endl;
    } else if (choice == static_cast<int>(DELETE_DATA)) {
      std::string parentKey;
      std::cout << "Enter parent key to delete: ";
      std::cin >> parentKey;
      jsonHandler.deleteData(parentKey);
    } else if (choice == static_cast<int>(EXIT)) {
      break;
    } else {
      std::cout << "Invalid choice. Please try again." << std::endl;
    }
  }

  return 0;
}
