/**
 * @file ServoHat.cpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#include "i2c/ServoHat.hpp"

i2c::ServoHat::ServoHat(const gsl::not_null<std::shared_ptr<Bus>> &bus, int f) :
        frequency(f),
        driver(bus, PCA9685::DEFAULT_ADDRESS, frequency) {
    driver.setOnTimeGlobal(0);
}

void i2c::ServoHat::setServo(gsl::index servo, unsigned int microseconds) {
    driver.setOffTime(servo, counts(microseconds));
}

void i2c::ServoHat::turnAllOff() {
    driver.setAllOff();
}
