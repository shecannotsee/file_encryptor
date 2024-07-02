#include "json_handler.h"

#include <fstream>
#include <iostream>

#include "encryption.h"

JsonHandler::JsonHandler(const std::string &filePath, const std::string &password)
    : filePath(filePath), password(password) {
}

bool JsonHandler::loadJson() {
  std::ifstream file(filePath);
  if (file.is_open()) {
    file >> jsonData;
    file.close();
    return verifyPassword();
  } else {
    jsonData["filePasswordEncrypted"] = encrypt(password, password);
    jsonData["data_ids"]              = nlohmann::json::array();
    return saveJson();
  }
}

bool JsonHandler::saveJson() {
  std::ofstream file(filePath);
  if (file.is_open()) {
    file << jsonData.dump(4);
    file.close();
    return true;
  }
  return false;
}

bool JsonHandler::verifyPassword() {
  std::string encryptedPassword = jsonData["filePasswordEncrypted"];
  std::string decryptedPassword = encrypt(encryptedPassword, password);

  std::cout << "Decrypted Password: \n" << encryptedPassword << "\n" << decryptedPassword << std::endl;

  return decryptedPassword == password;
}

void JsonHandler::addData(const std::string &parentKey,
                          const std::string &childKey,
                          const std::string &value,
                          bool encryptData) {
  nlohmann::json newData = value;
  if (encryptData) {
    newData = encrypt(newData, password);
  }

  // Ensure parentKey exists and is an object
  if (!jsonData.contains(parentKey)) {
    jsonData[parentKey] = nlohmann::json::object();
  }

  // Update data_ids
  if (!jsonData["data_ids"].contains(parentKey)) {
    jsonData["data_ids"].push_back(parentKey);
  }

  // Add or update childKey under parentKey
  jsonData[parentKey][childKey] = newData;

  saveJson();
}

nlohmann::json JsonHandler::getData(const std::string &parentKey, const std::string &childKey, bool encryptData) {
  nlohmann::json newData = jsonData[parentKey][childKey];
  if (encryptData) {
    newData = encrypt(newData, password);
  }
  return newData;
}

void JsonHandler::deleteData(const std::string &parentKey) {
  if (jsonData.contains(parentKey)) {
    jsonData[parentKey] = nlohmann::json();
    saveJson();
  }
}
