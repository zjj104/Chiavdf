#include "c_interface.h"
#include "imp_vdf.hpp"

#include <cstring>
char* c_create_discriminant(const char* challenge_hash, int discriminant_size_bits)
{
    Cvdf vdf;
    std::string str = vdf.create_discriminant(challenge_hash, discriminant_size_bits);
    
    char *c = (char*)malloc(str.length() + 1);
    strcpy(c, str.c_str());
    return c;
}

bool c_verify_wesolowski(const char* discriminant, const char* x_s, const char* y_s, const char* proof_s, uint64_t num_iterations)
{
    Cvdf vdf;
    bool istrue = vdf.verify_wesolowski(discriminant, x_s, y_s, proof_s, num_iterations);
    return istrue;
}


bool c_verify_n_wesolowski(const char* discriminant, const char* x_s, const char* proof_blob, 
                        const uint64_t num_iterations, const uint64_t disc_size_bits, const uint64_t recursion)
{
    Cvdf vdf;
    bool istrue = vdf.verify_n_wesolowski(discriminant, x_s, proof_blob, num_iterations, disc_size_bits, recursion);
    return istrue;
}

s_verify_n_wesolowski_with_b c_verify_n_wesolowski_with_b(const char* discriminant, const char* B, const char* x_s, 
const char* proof_blob, const uint64_t num_iterations, const uint64_t recursion)
{
    Cvdf vdf;
    s_verify_n_wesolowski_with_b pair = vdf.verify_n_wesolowski_with_b(discriminant, B, x_s, proof_blob, num_iterations, recursion); // pair.y_from_compression is new in c++
    return pair;
}

char* c_get_b_from_n_wesolowski(const char* discriminant, const char* x_s, const char* proof_blob, const uint64_t num_iterations, const uint64_t recursion)
{
    Cvdf vdf;
    std::string result = vdf.get_b_from_n_wesolowski(discriminant, x_s, proof_blob, num_iterations, recursion);

    char *c = (char*)malloc(result.length() + 1);
    strcpy(c, result.c_str());
    return c;
}

char* c_prove(const char* challenge_hash, const char* x_s, int discriminant_size_bits, uint64_t num_iterations)
{
    Cvdf vdf;
    std::string string = vdf.prove(challenge_hash, x_s, discriminant_size_bits, num_iterations);

    char* c = (char*)malloc(string.length() + 1);
    strcpy(c, string.c_str());
    return c;
}
/*
std::vector<uint8_t> c_generate_discriminant_challenge()
{
    Cvdf vdf;
    std::vector<uint8_t> discriminant_challenge = vdf.generate_discriminant_challenge();
    return discriminant_challenge;
}
*/