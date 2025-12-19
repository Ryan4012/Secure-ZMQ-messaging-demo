#include "crypto_utils.hpp"
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <vector>
#include <stdexcept>


std::string base64_encode(const unsigned char* buffer, size_t length) {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // No newlines
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, buffer, length);
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string encoded(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return encoded;
}

std::string base64_decode(const std::string& encoded) {
    BIO *bio, *b64;
    int decodeLen = (int)encoded.length();
    std::string decoded(decodeLen, '\0');

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // No newlines
    bio = BIO_new_mem_buf(encoded.data(), decodeLen);
    bio = BIO_push(b64, bio);

    int length = BIO_read(bio, &decoded[0], decodeLen);
    decoded.resize(length);

    BIO_free_all(bio);
    return decoded;
}



// Signing Function
std::string sign_message(const std::string& message, 
                         const std::string& private_key_path) {

    // Fail early if key is missing
    FILE* fp = fopen(private_key_path.c_str(), "r");
    if (!fp)
        throw std::runtime_error("Unable to open private key file");
        
    // Loads RSA key into OpenSSL structure
    EVP_PKEY* private_key = PEM_read_PrivateKey(fp, nullptr, nullptr, nullptr);
    fclose(fp);

    // Create digest content, uses SHA-256, binds private key
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestSignInit(ctx, nullptr, EVP_sha256(), nullptr, private_key);

    // Feeds data to be signed
    EVP_DigestSignUpdate(ctx, message.data(), message.size());

    // First call determines signature size
    size_t sig_len;
    EVP_DigestSignFinal(ctx, nullptr, &sig_len);

    // Second call actually writes signature
    std::vector<unsigned char> sig(sig_len);
    EVP_DigestSignFinal(ctx, sig.data(), &sig_len);

    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(private_key);

    return std::string(reinterpret_cast<char*>(sig.data()), sig_len);
}

// Verification Function * Same steps but in revers *
bool verify_signature(const std::string& message,
                      const std::string& signature,
                      const std::string& public_key_path) {

    // Loads public key
    FILE* fp = fopen(public_key_path.c_str(), "r");
    EVP_PKEY* pub_key = PEM_read_PUBKEY(fp, nullptr, nullptr, nullptr);
    fclose(fp);

    // Initialize verify context
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestVerifyInit(ctx, nullptr, EVP_sha256(), nullptr, pub_key);

    // Feeds original message
    EVP_DigestVerifyUpdate(ctx, message.data(), message.size());

    // Returns 1 if valid and 0 or <0 if invalid
    int result = EVP_DigestVerifyFinal(
        ctx,
        reinterpret_cast<const unsigned char*>(signature.data()),
        signature.size()
    );

    EVP_MD_CTX_free(ctx);
    EVP_PKEY_free(pub_key);

    return result == 1;
}
