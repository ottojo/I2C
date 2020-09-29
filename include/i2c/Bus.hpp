/**
 * @file Bus.hpp
 * @author jonas
 * @date 9/5/20
 * Description here TODO
 */

#ifndef I2C_BUS_HPP
#define I2C_BUS_HPP

#include <filesystem>
#include <fstream>
#include <mutex>
#include <array>
#include <unistd.h>


namespace i2c {
    class Bus {
            using FD = int;
        public:
            explicit Bus(const std::filesystem::path &path);

            Bus(const Bus &) = delete;

            Bus &operator=(const Bus &) = delete;


        private:
            FD fileDescriptor;
            std::mutex busMutex;

            class LockedBus {
                    friend class Bus;

                private:
                    LockedBus(std::mutex &m, Bus &b, int deviceAddress);

                    std::lock_guard<std::mutex> lock;
                    Bus &bus;

                public:
                    std::byte read() {
                        return read < 1 > ()[0];
                    };

                    template<size_t N>
                    std::array<std::byte, N> read() {
                        std::array<std::byte, N> result{};
                        auto readed = ::read(bus.fileDescriptor, &result, N);
                        if (readed != N) {
                            throw std::runtime_error{"Error reading from I2C device"};
                        }
                        return result;
                    }

                    std::byte readFrom(std::byte reg) {
                        write(reg);
                        return read<1>()[0];
                    };

                    template<size_t N>
                    std::array<std::byte, N> readFrom(std::byte reg) {
                        write(reg);
                        std::array<std::byte, N> result{};
                        auto readed = ::read(bus.fileDescriptor, &result, N);
                        if (readed != N) {
                            throw std::runtime_error{"Error reading from I2C device"};
                        }
                        return result;
                    }

                    void write(std::byte data) {
                        write < 1 > ({ data });
                    }

                    template<size_t N>
                    void write(const std::array<std::byte, N> &data) {
                        auto written = ::write(bus.fileDescriptor, data.data(), N);
                        if (written != N) {
                            throw std::runtime_error{"Error writing to I2C device"};
                        }
                    }
            };

        public:
            LockedBus lock(int deviceAddress);
    };
}

#endif //I2C_BUS_HPP
