/**
 * @file util.hpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#ifndef I2C_I2CUTIL_HPP
#define I2C_I2CUTIL_HPP

#include <cstdint>
#include <cstddef>

namespace i2c::util {
    constexpr std::byte highByte(const uint16_t &val) {
        return std::byte(val >> 8U);
    }

    constexpr std::byte lowByte(const uint16_t &val) {
        return std::byte(val & 0xFFU);
    }
}
#endif //I2C_I2CUTIL_HPP
