/**
 * @file PCA9685.hpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#ifndef I2C_PCA9685_HPP
#define I2C_PCA9685_HPP

#include <stdint.h>             // for uint16_t
#include <cstddef>              // for byte
#include <gsl/pointers>         // for not_null
#include <memory>               // for shared_ptr
#include "i2c/byteLiteral.hpp"  // for operator""_byte
#include "Bus.hpp"              // IWYU pragma: keep

namespace i2c {
    class PCA9685 {
        public:
            explicit PCA9685(const gsl::not_null<std::shared_ptr<Bus>> &bus,
                             int address = DEFAULT_ADDRESS,
                             int frequency = 200);

            void sleep();

            void restart();

            void setOnTime(int led, uint16_t time);

            void setOnTimeGlobal(uint16_t time);

            void setOffTime(int led, uint16_t time);

            void setOffTimeGlobal(uint16_t time);

            void setAllOff();

            static constexpr int DEFAULT_ADDRESS = 0x40;
            static constexpr double OSC_FREQ = 25'000'000.0;
            static constexpr int COUNTER_MAX = 4096;

        private:
            const int address;

            gsl::not_null<std::shared_ptr<Bus>> bus;

            static constexpr std::byte REG_MODE1 = 0x00_byte;
            static constexpr std::byte REG_PRESCALER = 0xFE_byte;
            static constexpr std::byte REG_ALL_LED_ON_L = 0xFA_byte;
            static constexpr std::byte REG_ALL_LED_OFF_L = 0xFC_byte;

            static constexpr std::byte REG_LED_OFF_L(int n) {
                return std::byte(0x08 + n * 4);
            }

            static constexpr std::byte REG_LED_ON_L(int n) {
                return std::byte(0x06 + n * 4);
            }


            static constexpr std::byte mode1(bool restart, bool extclk, bool ai, bool sleep);


            void setPrescaler(std::byte prescaler);
    };
}


#endif //I2C_PCA9685_HPP
