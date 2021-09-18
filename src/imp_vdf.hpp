#ifndef IMP_VDF_HPP
#define IMP_VDF_HPP

#include <iostream>
#include <vector>

#include "c_interface.h"

class Cvdf 
{
public:
    std::string create_discriminant(const std::string& challenge_hash, int discriminant_size_bits);
    bool verify_wesolowski(const std::string& discriminant, const std::string& x_s, const std::string& y_s, const std::string& proof_s, uint64_t num_iterations);
    bool verify_n_wesolowski(const std::string& discriminant, const std::string& x_s, const std::string& proof_blob, 
                        const uint64_t num_iterations, const uint64_t disc_size_bits, const uint64_t recursion);
    s_verify_n_wesolowski_with_b verify_n_wesolowski_with_b(const std::string& discriminant, const std::string& B, const std::string& x_s, 
                                        const std::string& proof_blob, const uint64_t num_iterations, const uint64_t recursion);
    std::string get_b_from_n_wesolowski(const std::string& discriminant, const std::string& x_s, 
                                        const std::string& proof_blob, const uint64_t num_iterations, const uint64_t recursion);
    std::string prove(const std::string& challenge_hash, const std::string& x_s, int discriminant_size_bits, uint64_t num_iterations);

    std::vector<uint8_t> generate_discriminant_challenge();
};

#endif // IMP_VDF_HPP