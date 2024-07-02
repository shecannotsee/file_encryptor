#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <nlohmann/json.hpp>
#include <string>

class JsonHandler {
 public:
  JsonHandler(const std::string &filePath, const std::string &password);
  bool loadJson();
  bool saveJson();
  bool verifyPassword();
  void addData(const std::string &parentKey,
               const std::string &childKey,
               const std::string &value,
               bool encryptData = false);
  nlohmann::json getData(const std::string &parentKey, const std::string &childKey,bool encryptData);
  void deleteData(const std::string &parentKey);

 private:
  std::string filePath;
  std::string password;
  nlohmann::json jsonData;
};

#endif  // JSON_HANDLER_H
