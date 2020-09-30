/**
 * @file Bus.cpp
 * @author jonas
 * @date 9/5/20
 * Description here TODO
 */

#include "i2c/Bus.hpp"      // for Bus, Bus::LockedBus
#include <cerrno>           // for errno
#include <cstring>          // for strerror
#include <fcntl.h>          // for open, O_RDWR
#include <filesystem>       // for path
#include <linux/i2c-dev.h>  // for I2C_SLAVE
#include <mutex>            // for mutex
#include <stdexcept>        // for runtime_error
#include <string>           // for allocator, basic_string, string
#include <sys/ioctl.h>      // for ioctl


namespace i2c {

    Bus::Bus(const std::filesystem::path &path) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, hicpp-vararg, hicpp-signed-bitwise)
        fileDescriptor = open(path.c_str(), O_RDWR | O_CLOEXEC);
        if (fileDescriptor < 0) {
            std::string errorMessage = "Error opening I2C bus: ";
            errorMessage += strerror(errno);
            throw std::runtime_error{errorMessage};
        }
    }

    Bus::LockedBus Bus::lock(int deviceAddress) {
        return i2c::Bus::LockedBus(busMutex, *this, deviceAddress);
    }

    Bus::LockedBus::LockedBus(std::mutex &m, Bus &b, int deviceAddress) : lock{m}, bus{b} {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, hicpp-vararg)
        if (ioctl(b.fileDescriptor, I2C_SLAVE, deviceAddress) < 0) {
            std::string errorMessage = "Error setting device address: ";
            errorMessage += strerror(errno);
            throw std::runtime_error{errorMessage};
        }
    }

} // namespace i2c
