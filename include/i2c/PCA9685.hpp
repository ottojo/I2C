/**
 * @file PCA9685.hpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#ifndef I2C_PCA9685_HPP
#define I2C_PCA9685_HPP

#include "i2c/Bus.hpp"
#include <gsl/gsl>
#include "i2c/byteLiteral.hpp"

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

            ~PCA9685();

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


            static constexpr std::byte mode1(bool restart, bool extclk, bool ai, bool sleep,
                                             bool sub1 = false, bool sub2 = false, bool sub3 = false,
                                             bool allcall = false);


            void setPrescaler(std::byte prescaler);
    };
}


#endif //I2C_PCA9685_HPP
