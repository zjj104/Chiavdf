#include "include.h"
#include "imp_vdf.hpp"
#include "integer_common.h"
#include "create_discriminant.h"
#include "verifier.h"
#include "prover_slow.h"

#include <iostream>
#include <vector>

// #define CH_SIZE 32
// #define LENGTH 1024

std::vector<uint8_t> HexToVectorBytes(const std::string& hex) 
{
    std::vector<uint8_t> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2) 
    {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

char* HexToCharByte(const std::string& hex) 
{
    std::vector<uint8_t> bytes; 

    for (unsigned int i = 0; i < hex.length(); i += 2) 
    {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    char* res = new char[bytes.size() + 1]; // +1 for the final 0
    res[bytes.size() + 1] = 0; // terminate the string
    for (size_t i = 0; i < bytes.size(); ++i) 
    {
        res[i] = bytes[i];
    }
    return res;
}


string Cvdf::create_discriminant(const std::string& challenge_hash, int discriminant_size_bits)
{
    std::vector<uint8_t> challenge_hash_bytes = HexToVectorBytes(challenge_hash);
    integer discr = CreateDiscriminant(challenge_hash_bytes, discriminant_size_bits);
    return discr.to_string();
}

bool Cvdf::verify_wesolowski(const string& discriminant, const string& x_s, const string& y_s, const string& proof_s, uint64_t num_iterations)
{
    integer D(discriminant);

    char* x_s_charByte = HexToCharByte(x_s);
    char* y_s_charByte = HexToCharByte(y_s);
    char* proof_s_charByte = HexToCharByte(proof_s);

    int x_s_size = x_s.size() / 2; 
    int y_s_size = y_s.size() / 2;
    int proof_s_size = proof_s.size() / 2;

    form x = DeserializeForm(D, (const uint8_t *)x_s_charByte, x_s_size);
    form y = DeserializeForm(D, (const uint8_t *)y_s_charByte, y_s_size);
    form proof = DeserializeForm(D, (const uint8_t *)proof_s_charByte, proof_s_size);

    bool is_valid = false;
    VerifyWesolowskiProof(D, x, y, proof, num_iterations, is_valid);
    return is_valid;
}

bool Cvdf::verify_n_wesolowski(const string& discriminant, const string& x_s, const string& proof_blob, 
const uint64_t num_iterations, const uint64_t disc_size_bits, const uint64_t recursion)
{
        char* x_s_charByte = HexToCharByte(x_s);
        char* proof_blob_charByte = HexToCharByte(proof_blob);
        int proof_blob_size = proof_blob.size() / 2;

        return CheckProofOfTimeNWesolowski(integer(discriminant), (const uint8_t *)x_s_charByte, (const uint8_t *)proof_blob_charByte, proof_blob_size, num_iterations, recursion);
}

s_verify_n_wesolowski_with_b Cvdf::verify_n_wesolowski_with_b(const string& discriminant, const string& B, const string& x_s, const string& proof_blob, const uint64_t num_iterations, const uint64_t recursion) 
{
    char* x_s_charByte = HexToCharByte(x_s);
    char* proof_blob_ptr_charByte = HexToCharByte(proof_blob);
    int proof_blob_size = proof_blob.size() / 2 ;

    std::pair<bool, std::vector<uint8_t>> result;
    result = CheckProofOfTimeNWesolowskiWithB(integer(discriminant), integer(B), (const uint8_t *)x_s_charByte, (const uint8_t *)proof_blob_ptr_charByte, proof_blob_size, num_iterations, recursion);
    
    std::string output;
    char ch[3];
    for(int i = 0; i < result.second.size(); i++)
    {
        sprintf(ch, "%02x", result.second[i]);
        output += ch;
    }

/***C***
    s_verify_n_wesolowski_with_b verify_struct;
    verify_struct.is_valid = result.first;

    const char* output_c_str = output.c_str();
	int tail = 0;
	while (output_c_str[tail] != '\0') {
		tail++;
	}
    char* real_result = (char*)malloc(tail + 1);
	for (int i = 0; i <= tail; i++) {
		real_result[i] = output_c_str[i];
	}

    verify_struct.y_from_compression = real_result;
*/

    s_verify_n_wesolowski_with_b verify_struct;
    verify_struct.is_valid = result.first;

    char *new_output = new char[output.length() + 1];
    memcpy(new_output, output.c_str(), output.length() + 1);

    verify_struct.y_from_compression = new_output;

    return verify_struct;
}

string Cvdf::get_b_from_n_wesolowski(const string& discriminant, const string& x_s, const string& proof_blob, const uint64_t num_iterations, const uint64_t recursion)
{
    char* x_s_charByte = HexToCharByte(x_s);
    char* proof_blob_charByte = HexToCharByte(proof_blob);
    int proof_blob_size = proof_blob.size() / 2;

    integer B = GetBFromProof(integer(discriminant), (const uint8_t *)x_s_charByte, (const uint8_t *)proof_blob_charByte, proof_blob_size, num_iterations, recursion);
    return B.to_string();
}

std::string Cvdf::prove(const string& challenge_hash, const string& x_s, int discriminant_size_bits, uint64_t num_iterations)
{
    char* res = HexToCharByte(x_s);
/*
typedef unsigned char BYTE;
BYTE initial_el[100] = { 
    0x08, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
*/
    std::vector<uint8_t> challenge_hash_bytes = HexToVectorBytes(challenge_hash);
    integer D = CreateDiscriminant(challenge_hash_bytes, discriminant_size_bits);

    form x = DeserializeForm(D, (const uint8_t *)res, 100);
    auto result = ProveSlow(D, x, num_iterations);
    
    string output;
    char ch[3];
    for(int i = 0; i < result.size(); i++)
    {
        sprintf(ch, "%02x", result[i]);
        output += ch;
    }
    return output;
}

/*******************************************/
std::vector<uint8_t> Cvdf::generate_discriminant_challenge()
{
    unsigned long low = 0;
    unsigned long limit = 1 << 8;
    unsigned long range = limit - low;

    std::random_device rd;  
    std::mt19937 rng(rd());   
    std::uniform_int_distribution<unsigned long> dist(low, limit - 1); 

    std::vector<uint8_t> discriminant_challenge;
    
    for(int i = 0; i < 32; i++)
    {
        unsigned long n = dist(rng);
        discriminant_challenge.push_back(n);
    }
    return discriminant_challenge;
}


