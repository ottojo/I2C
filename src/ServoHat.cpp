/**
 * @file ServoHat.cpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#include "i2c/Bus.hpp"       // for Bus
#include "i2c/PCA9685.hpp"   // for PCA9685, PCA9685::DEFAULT_ADDRESS
#include "i2c/ServoHat.hpp"  // for ServoHat
#include <bits/exception.h>  // for exception
#include <gsl/pointers>      // for not_null
#include <iostream>          // for operator<<, basic_ostream, endl, cerr
#include <memory>            // for shared_ptr

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

i2c::ServoHat::~ServoHat() {
    try {
        turnAllOff();
    } catch (std::exception &e) {
        std::cerr << "Could not turn off servos! " << e.what() << std::endl;
    }
}
