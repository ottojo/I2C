/**
 * @file PCA9685.cpp
 * @author jonas
 * @date 9/6/20
 * Description here TODO
 */

#include "i2c/PCA9685.hpp"
#include "i2c/Bus.hpp"      // for Bus, Bus::LockedBus
#include "i2c/i2cutil.hpp"  // for highByte, lowByte
#include <chrono>           // for microseconds
#include <cmath>            // for round
#include <thread>           // for sleep_for

i2c::PCA9685::PCA9685(const gsl::not_null<std::shared_ptr<Bus>> &bus, int address, int frequency) :
        address(address),
        bus(bus) {

    sleep();
    auto prescaler = static_cast<uint8_t>(std::round(OSC_FREQ / (COUNTER_MAX * frequency))) - 1;
    setPrescaler(std::byte(prescaler));
    restart();
}

constexpr std::byte i2c::PCA9685::mode1(bool restart, bool extclk, bool ai, bool sleep) {
    std::byte result{0};
    constexpr auto OFFSET_RESTART = 7U;
    constexpr auto OFFSET_EXTCLK = 6U;
    constexpr auto OFFSET_AI = 5U;
    constexpr auto OFFSET_SLEEP = 4U;

    result |= std::byte(static_cast<unsigned int>(restart) << OFFSET_RESTART);
    result |= std::byte(static_cast<unsigned int>(extclk) << OFFSET_EXTCLK);
    result |= std::byte(static_cast<unsigned int>(ai) << OFFSET_AI);
    result |= std::byte(static_cast<unsigned int>(sleep) << OFFSET_SLEEP);
    return result;
}

void i2c::PCA9685::sleep() {
    bus->lock(address).write<2>({REG_MODE1, mode1(false, false, true, true)});
}

void i2c::PCA9685::restart() {
    // clear sleep bit
    bus->lock(address).write<2>({REG_MODE1, mode1(false, false, true, false)});

    // let oscillator stabilize
    constexpr auto OSC_RECOMMENDED_STABILIZE_TIME = 500;
    std::this_thread::sleep_for(std::chrono::microseconds{OSC_RECOMMENDED_STABILIZE_TIME});

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

void i2c::PCA9685::setAllOff() {
    constexpr unsigned int OFF_MAGIC_OFFSET = 12U;
    setOffTimeGlobal(1U << OFF_MAGIC_OFFSET);
}
