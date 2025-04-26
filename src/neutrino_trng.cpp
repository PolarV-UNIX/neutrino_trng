#include "../include/neutrino_trng/neutrino_trng.h"
#include <chrono>
#include <thread>
#include <random>
#include <vector>
#include <openssl/sha.h>
#include <openssl/evp.h>

using namespace std;

vector<uint8_t> logistic_map(int length = 512, double x = 0.7312, double r = 3.99){
    vector<uint8_t> chaos;
    for (int i = 0; i < length; i++){
        x = r * x * (1 - x);
        chaos.push_back(static_cast<uint8_t>(x * 256));
    }
    return chaos;
}

string hash_sha3_512(const vector<uint8_t>& data){
    unsigned char hash[64];
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha3_512(), nullptr);
    EVP_DigestUpdate(ctx, data.data(), data.size());
    EVP_DigestFinal_ex(ctx, hash, nullptr);
    EVP_MD_CTX_free(ctx);

    string hex;
    char buf[3];
    for (int i =0; i < 64; i++){
        sprintf(buf, "%02x", hash[i]);
        hex += buf;
    }
    return hex;
}

const char* generate_neutrino_hash() {
    static string result;

    auto now = chrono::high_resolution_clock::now().time_since_epoch().count();
    auto cpu_load = rand() % 100;

    vector<uint8_t> entropy;
    for (int i = 0; i < 256; ++i){
        entropy.push_back((now >> (i % 64)) & 0xFF);
        entropy.push_back(cpu_load ^ (i * 31));
    }

    auto chaos = logistic_map();
    for (size_t i = 0; i < entropy.size(); i++){
        entropy[i] ^= chaos[i % chaos.size()];
    }

    result = hash_sha3_512(entropy);
    return result.c_str();
}
