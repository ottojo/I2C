/**
 * @file Bus.cpp
 * @author jonas
 * @date 9/5/20
 * Description here TODO
 */

#include <iostream>
#include <fcntl.h>
#include <cstring>
#include "i2c/Bus.hpp"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


namespace i2c {

    Bus::Bus(const std::filesystem::path &path) {
        fileDescriptor = open(path.c_str(), O_RDWR);
        if (fileDescriptor < 0) {
            auto *errorMessage = new std::string("Error opening I2C bus: ");
            *errorMessage += strerror(errno);
            throw std::runtime_error{errorMessage->c_str()};
        }
    }

    Bus::LockedBus Bus::lock(int deviceAddress) {
        return i2c::Bus::LockedBus(busMutex, *this, deviceAddress);
    }

    Bus::LockedBus::LockedBus(std::mutex &m, Bus &b, int deviceAddress) : lock{m}, bus{b} {
        if (ioctl(b.fileDescriptor, I2C_SLAVE, deviceAddress) < 0) {
            auto *errorMessage = new std::string("Error setting device address: ");
            *errorMessage += strerror(errno);
            throw std::runtime_error{errorMessage->c_str()};
        }
    }

}
