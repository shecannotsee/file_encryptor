#ifndef EN_DE_H
#define EN_DE_H

#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/evp.h>

#include <cstring>
#include <fstream>
#include <vector>

void handleErrors() {
  ERR_print_errors_fp(stderr);
  abort();
}

void deriveKeyAndIV(const std::string &password, unsigned char *key, unsigned char *iv) {
  const EVP_MD *dgst = EVP_sha256();
  if (!dgst) {
    handleErrors();
  }
  if (!EVP_BytesToKey(EVP_aes_256_cbc(), dgst, nullptr, (unsigned char *)password.c_str(), password.length(), 1, key,
                      iv)) {
    handleErrors();
  }
}

void encryptFile(const std::string &filename, const std::string &password) {
  std::ifstream infile(filename, std::ios::binary);
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
  infile.close();

  unsigned char key[32], iv[16];
  deriveKeyAndIV(password, key, iv);

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) handleErrors();

  if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) handleErrors();

  std::vector<unsigned char> ciphertext(buffer.size() + AES_BLOCK_SIZE);
  int len;
  if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, buffer.data(), buffer.size())) handleErrors();

  int ciphertext_len = len;
  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) handleErrors();
  ciphertext_len += len;

  ciphertext.resize(ciphertext_len);

  EVP_CIPHER_CTX_free(ctx);

  std::ofstream outfile(filename + ".enc", std::ios::binary);
  outfile.write((char *)iv, sizeof(iv));
  outfile.write((char *)ciphertext.data(), ciphertext.size());
  outfile.close();
}

void decryptFile(const std::string &filename, const std::string &password) {
  std::ifstream infile(filename, std::ios::binary);
  std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
  infile.close();

  unsigned char key[32], iv[16];
  std::memcpy(iv, buffer.data(), sizeof(iv));
  buffer.erase(buffer.begin(), buffer.begin() + sizeof(iv));
  deriveKeyAndIV(password, key, iv);

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) handleErrors();

  if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv)) handleErrors();

  std::vector<unsigned char> plaintext(buffer.size());
  int len;
  if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, buffer.data(), buffer.size())) handleErrors();

  int plaintext_len = len;
  if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
  plaintext_len += len;

  plaintext.resize(plaintext_len);

  EVP_CIPHER_CTX_free(ctx);

  std::ofstream outfile(filename + ".dec", std::ios::binary);
  outfile.write((char *)plaintext.data(), plaintext.size());
  outfile.close();
}

#endif //EN_DE_H
