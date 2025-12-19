#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <string>

// Signs a message using a private key
std::string sign_message(const std::string& message,
                         const std::string& private_key_path);

// Verifies a signature using a public key
bool verify_signature(const std::string& message,
                      const std::string& signature,
                      const std::string& public_key_path);

// Base64 helpers
std::string base64_encode(const unsigned char* buffer, size_t length);
std::string base64_decode(const std::string& encoded);

#endif
