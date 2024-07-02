#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

std::string encrypt(const std::string &plainText, const std::string &key);
std::string decrypt(const std::string &cipherText, const std::string &key);
std::string base64_encode(const std::string &in);
std::string base64_decode(const std::string &in);

#endif // ENCRYPTION_H
