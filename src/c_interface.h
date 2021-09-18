#ifndef C_INTERFACE_H
#define C_INTERFACE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct 
{
    bool is_valid;
    const char* y_from_compression;
}s_verify_n_wesolowski_with_b;

#ifdef __cplusplus
extern "C" {
#endif
char* c_create_discriminant(const char* challenge_hash, int discriminant_size_bits);
bool c_verify_wesolowski(const char* discriminant, const char* x_s, const char* y_s, const char* proof_s, uint64_t num_iterations);
bool c_verify_n_wesolowski(const char* discriminant, const char* x_s, const char* proof_blob, 
                        const uint64_t num_iterations, const uint64_t disc_size_bits, const uint64_t recursion);
s_verify_n_wesolowski_with_b c_verify_n_wesolowski_with_b(const char* discriminant, const char* B, const char* x_s, 
                                       const char* proof_blob, const uint64_t num_iterations, const uint64_t recursion);
char* c_get_b_from_n_wesolowski(const char* discriminant, const char* x_s, const char* proof_blob, const uint64_t num_iterations, const uint64_t recursion);
char* c_prove(const char* challenge_hash, const char* x_s, int discriminant_size_bits, uint64_t num_iterations);

// std::vector<uint8_t> c_generate_discriminant_challenge();
#ifdef __cplusplus
}
#endif 

#endif // C_INTERFACE_H


