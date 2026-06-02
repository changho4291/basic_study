#ifndef WSSE_USERNAME_TOKEN_H
#define WSSE_USERNAME_TOKEN_H

#include <cstring>
#include <ctime>
#include <string>
#include <vector>

#include <openssl/evp.h>
#include <openssl/rand.h>

namespace wsse_auth {

constexpr const char* kPasswordDigestType =
    "http://docs.oasis-open.org/wss/2004/01/"
    "oasis-200401-wss-username-token-profile-1.0#PasswordDigest";
constexpr const char* kBase64BinaryType =
    "http://docs.oasis-open.org/wss/2004/01/"
    "oasis-200401-wss-soap-message-security-1.0#Base64Binary";

inline char* soap_strdup_owned(soap* soap, const std::string& value) {
    char* copy = static_cast<char*>(soap_malloc(soap, value.size() + 1));
    if (copy == nullptr) {
        return nullptr;
    }

    std::memcpy(copy, value.c_str(), value.size() + 1);
    return copy;
}

inline std::string base64_encode(const unsigned char* data, int size) {
    std::string encoded(4 * ((size + 2) / 3), '\0');
    const int out_size = EVP_EncodeBlock(
        reinterpret_cast<unsigned char*>(&encoded[0]),
        data,
        size);
    encoded.resize(out_size);
    return encoded;
}

inline bool base64_decode(const char* text, std::vector<unsigned char>& decoded) {
    if (text == nullptr) {
        return false;
    }

    const int input_size = static_cast<int>(std::strlen(text));
    if (input_size == 0 || input_size % 4 != 0) {
        return false;
    }

    decoded.assign(3 * (input_size / 4), 0);
    const int out_size = EVP_DecodeBlock(
        decoded.data(),
        reinterpret_cast<const unsigned char*>(text),
        input_size);
    if (out_size < 0) {
        return false;
    }

    int padding = 0;
    if (input_size >= 1 && text[input_size - 1] == '=') {
        ++padding;
    }
    if (input_size >= 2 && text[input_size - 2] == '=') {
        ++padding;
    }

    decoded.resize(out_size - padding);
    return true;
}

inline std::string utc_now() {
    std::time_t now = std::time(nullptr);
    std::tm utc {};
    gmtime_r(&now, &utc);

    char buffer[32] {};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &utc);
    return buffer;
}

inline std::string make_password_digest(
    const unsigned char* nonce,
    int nonce_size,
    const std::string& created,
    const char* password) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    unsigned char hash[EVP_MAX_MD_SIZE] {};
    unsigned int hash_size = 0;

    if (ctx == nullptr ||
        EVP_DigestInit_ex(ctx, EVP_sha1(), nullptr) != 1 ||
        EVP_DigestUpdate(ctx, nonce, nonce_size) != 1 ||
        EVP_DigestUpdate(ctx, created.data(), created.size()) != 1 ||
        EVP_DigestUpdate(ctx, password, std::strlen(password)) != 1 ||
        EVP_DigestFinal_ex(ctx, hash, &hash_size) != 1) {
        EVP_MD_CTX_free(ctx);
        return {};
    }

    EVP_MD_CTX_free(ctx);
    return base64_encode(hash, static_cast<int>(hash_size));
}

inline bool add_username_token_digest(
    soap* soap,
    const char* username,
    const char* password) {
    unsigned char nonce[16] {};
    if (RAND_bytes(nonce, sizeof(nonce)) != 1) {
        return false;
    }

    const std::string nonce_b64 = base64_encode(nonce, sizeof(nonce));
    const std::string created = utc_now();
    const std::string digest = make_password_digest(nonce, sizeof(nonce), created, password);
    if (digest.empty()) {
        return false;
    }

    if (soap->header == nullptr) {
        soap->header = soap_new_SOAP_ENV__Header(soap);
    }

    soap->header->wsse__Security = soap_new__wsse__Security(soap);
    soap->header->wsse__Security->UsernameToken = soap_new__wsse__UsernameToken(soap);

    _wsse__UsernameToken* token = soap->header->wsse__Security->UsernameToken;
    token->wsu__Id = soap_strdup_owned(soap, "UsernameToken");
    token->Username = soap_strdup_owned(soap, username);
    token->wsu__Created = soap_strdup_owned(soap, created);

    token->Password = soap_new__wsse__Password(soap);
    token->Password->__item = soap_strdup_owned(soap, digest);
    token->Password->Type = soap_strdup_owned(soap, kPasswordDigestType);

    token->Nonce = soap_new_wsse__EncodedString(soap);
    token->Nonce->__item = soap_strdup_owned(soap, nonce_b64);
    token->Nonce->EncodingType = soap_strdup_owned(soap, kBase64BinaryType);

    return token->wsu__Id != nullptr &&
           token->Username != nullptr &&
           token->wsu__Created != nullptr &&
           token->Password->__item != nullptr &&
           token->Password->Type != nullptr &&
           token->Nonce->__item != nullptr &&
           token->Nonce->EncodingType != nullptr;
}

inline _wsse__UsernameToken* get_username_token(soap* soap) {
    if (soap == nullptr ||
        soap->header == nullptr ||
        soap->header->wsse__Security == nullptr) {
        return nullptr;
    }

    return soap->header->wsse__Security->UsernameToken;
}

inline bool verify_username_token_digest(
    soap* soap,
    const char* expected_username,
    const char* expected_password,
    std::string& reason) {
    _wsse__UsernameToken* token = get_username_token(soap);
    if (token == nullptr) {
        reason = "missing UsernameToken";
        return false;
    }

    if (token->Username == nullptr || std::strcmp(token->Username, expected_username) != 0) {
        reason = "unknown username";
        return false;
    }

    if (token->Password == nullptr || token->Password->__item == nullptr) {
        reason = "missing PasswordDigest";
        return false;
    }

    if (token->Nonce == nullptr || token->Nonce->__item == nullptr) {
        reason = "missing Nonce";
        return false;
    }

    if (token->wsu__Created == nullptr) {
        reason = "missing Created";
        return false;
    }

    std::vector<unsigned char> nonce;
    if (!base64_decode(token->Nonce->__item, nonce)) {
        reason = "invalid Nonce base64";
        return false;
    }

    const std::string expected_digest = make_password_digest(
        nonce.data(),
        static_cast<int>(nonce.size()),
        token->wsu__Created,
        expected_password);
    if (expected_digest.empty()) {
        reason = "digest calculation failed";
        return false;
    }

    if (expected_digest != token->Password->__item) {
        reason = "password digest mismatch";
        return false;
    }

    reason = "ok";
    return true;
}

}  // namespace wsse_auth

#endif
