/**
 * @file byteLiteral.hpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#ifndef I2C_BYTELITERAL_HPP
#define I2C_BYTELITERAL_HPP

#include <cstddef>
#include <gsl/gsl_narrow>

constexpr std::byte operator ""_byte(unsigned long long i) {
    return gsl::narrow<std::byte>(i);
}

#endif //I2C_BYTELITERAL_HPP
