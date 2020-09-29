/**
 * @file PCA9685.cpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#include "i2c/PCA9685.hpp"
#include "i2c/i2cutil.hpp"

#include <cmath>
#include <thread>
#include <fmt/core.h>

i2c::PCA9685::PCA9685(const gsl::not_null<std::shared_ptr<Bus>> &bus, int address, int frequency) :
        address(address),
        bus(bus) {

    sleep();
    auto prescaler = static_cast<uint8_t>(std::round(25'000'000.0 / (4096 * frequency))) - 1;
    setPrescaler(std::byte(prescaler));
    restart();
}

constexpr std::byte i2c::PCA9685::mode1(bool restart, bool extclk, bool ai, bool sleep,
                                        bool sub1, bool sub2, bool sub3, bool allcall) {
    std::byte result{0};
    result |= std::byte(restart << 7u);
    result |= std::byte(extclk << 6u);
    result |= std::byte(ai << 5u);
    result |= std::byte(sleep << 4u);
    result |= std::byte(sub1 << 3u);
    result |= std::byte(sub2 << 2u);
    result |= std::byte(sub3 << 1u);
    result |= std::byte(allcall);
    return result;
}

void i2c::PCA9685::sleep() {
    bus->lock(address).write<2>({REG_MODE1, mode1(false, false, true, true)});
}

void i2c::PCA9685::restart() {
    // clear sleep bit
    bus->lock(address).write<2>({REG_MODE1, mode1(false, false, true, false)});

    // let oscillator stabilize
    std::this_thread::sleep_for(std::chrono::microseconds{500});

    // restart
    bus->lock(address).write<2>({REG_MODE1, mode1(true, false, true, false)});
}

void i2c::PCA9685::setPrescaler(std::byte prescaler) {
    bus->lock(address).write<2>({REG_PRESCALER, prescaler});
}

void i2c::PCA9685::setOnTime(int led, uint16_t time) {
    bus->lock(address).write<3>({REG_LED_ON_L(led), util::lowByte(time), util::highByte(time)});
}

void i2c::PCA9685::setOffTime(int led, uint16_t time) {
    bus->lock(address).write<3>({REG_LED_OFF_L(led), util::lowByte(time), util::highByte(time)});
}

void i2c::PCA9685::setOnTimeGlobal(uint16_t time) {
    bus->lock(address).write<3>({REG_ALL_LED_ON_L, util::lowByte(time), util::highByte(time)});
}

void i2c::PCA9685::setOffTimeGlobal(uint16_t time) {
    bus->lock(address).write<3>({REG_ALL_LED_OFF_L, util::lowByte(time), util::highByte(time)});
}

i2c::PCA9685::~PCA9685() {
    setAllOff();
}

void i2c::PCA9685::setAllOff() {
    fmt::print("PCA9685: disabling all outputs\n");
    setOffTimeGlobal(1u << 12u);
}
