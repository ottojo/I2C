#include "i2c/Bus.hpp"
#include <thread>
#include "i2c/PCA9685.hpp"
int main(){

    i2c::PCA9685 p{std::make_shared<i2c::Bus>("/dev/i2c-1")};
    p.setOnTimeGlobal(0);
    p.setOffTimeGlobal(1228);
    std::this_thread::sleep_for(std::chrono::seconds{1});
    p.setOffTimeGlobal(0);


/*    i2c::ServoHat servoHat(std::make_shared<i2c::Bus>("/dev/i2c-1"), 200);

    constexpr auto min = 1500;
    constexpr auto max = 1550;
    constexpr auto delay = std::chrono::milliseconds{3};

    for (int i = 0; i < 10; i++) {
        for (unsigned int micros = min; micros < max; micros++) {
            servoHat.setServo(0, min);
            std::this_thread::sleep_for(delay);
        }
    }
    */
}
