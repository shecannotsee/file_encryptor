#include "encryption.h"

#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include <stdexcept>
#include <vector>

const int AES_KEYLEN = 256;
const int IV_SIZE    = 16;

std::string base64_encode(const std::string &in) {
  BIO *b64 = BIO_new(BIO_f_base64());
  BIO *bio = BIO_new(BIO_s_mem());
  b64      = BIO_push(b64, bio);
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  BIO_write(b64, in.data(), in.size());
  BIO_flush(b64);
  BUF_MEM *bufferPtr;
  BIO_get_mem_ptr(b64, &bufferPtr);
  std::string out(bufferPtr->data, bufferPtr->length);
  BIO_free_all(b64);
  return out;
}

std::string base64_decode(const std::string &in) {
  BIO *b64 = BIO_new(BIO_f_base64());
  BIO *bio = BIO_new_mem_buf(in.data(), in.size());
  b64      = BIO_push(b64, bio);
  BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
  std::vector<char> buffer(in.size());
  int decodedSize = BIO_read(b64, buffer.data(), buffer.size());
  BIO_free_all(b64);
  return std::string(buffer.data(), decodedSize);
}

std::string encrypt(const std::string &plainText, const std::string &key) {
  unsigned char iv[IV_SIZE];
  RAND_bytes(iv, IV_SIZE);

  std::vector<unsigned char> cipherText(plainText.size() + AES_KEYLEN / 8 + IV_SIZE);
  int outLen;

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), iv);
  EVP_EncryptUpdate(ctx, cipherText.data(), &outLen, reinterpret_cast<const unsigned char *>(plainText.c_str()),
                    plainText.size());
  int totalLen = outLen;
  EVP_EncryptFinal_ex(ctx, cipherText.data() + outLen, &outLen);
  totalLen += outLen;
  EVP_CIPHER_CTX_free(ctx);

  cipherText.resize(totalLen + IV_SIZE);
  std::copy(iv, iv + IV_SIZE, cipherText.begin() + totalLen);

  return base64_encode(std::string(cipherText.begin(), cipherText.end()));
}

std::string decrypt(const std::string &cipherText, const std::string &key) {
  std::string decodedCipherText = base64_decode(cipherText);
  unsigned char iv[IV_SIZE];
  std::copy(decodedCipherText.end() - IV_SIZE, decodedCipherText.end(), iv);

  std::vector<unsigned char> plainText(decodedCipherText.size());
  int outLen;

  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), iv);
  EVP_DecryptUpdate(ctx, plainText.data(), &outLen, reinterpret_cast<const unsigned char *>(decodedCipherText.data()),
                    decodedCipherText.size() - IV_SIZE);
  int totalLen = outLen;
  EVP_DecryptFinal_ex(ctx, plainText.data() + outLen, &outLen);
  totalLen += outLen;
  EVP_CIPHER_CTX_free(ctx);

  plainText.resize(totalLen);
  return std::string(plainText.begin(), plainText.end());
}
