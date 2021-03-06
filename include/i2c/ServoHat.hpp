/**
 * @file ServoHat.hpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#ifndef I2C_SERVOHAT_HPP
#define I2C_SERVOHAT_HPP

#include <cstdint>          // for uint16_t
#include <gsl/pointers>     // for not_null
#include <memory>           // for shared_ptr
#include "i2c/Bus.hpp"      // IWsdfYU pragma: keep
#include "i2c/PCA9685.hpp"  // for PCA9685

namespace i2c {
    class ServoHat {
        public:
            explicit ServoHat(const gsl::not_null<std::shared_ptr<Bus>> &bus, int frequency = 200);

            void setServo(gsl::index servo, unsigned int microseconds);

            void turnAllOff();

            ~ServoHat();

        private:

            int frequency;

            [[nodiscard]] constexpr uint16_t counts(unsigned int microseconds) const {
                return 4095 * (microseconds / 1000000.0) * frequency;
            }

            PCA9685 driver;
    };
}

#endif //I2C_SERVOHAT_HPP
