# I2C

## Installation
```bash
mkdir build
cd build
cmake ..
make
sudo make install
```

## Integration
### CMake
To use I2C in an executable `MyProgram`, your CMakeLists.txt would look like this:
```cmake
find_package(ottojo.I2C REQUIRED)

add_executable(MyProgram main.cpp)
target_link_libraries(MyProgram PRIVATE ottojo.I2C::I2C)
```

## Usage
The first step is always to instantiate the I2C Bus:
```c++
auto bus = std::make_shared<i2c::Bus>("/dev/i2c-1");
```

Before you can communicate with a target device, the Bus needs to be locked:
```c++
constexpr auto DEVICE_ADDRESS = 0x40;
bus.lock(DEVICE_ADDRESS).write<3>({std::byte{0xA}, std::byte{0xB}, std::byte{0xC}});
```

While the Bus is locked, no other thread can access the Bus.
If you need to write to the device, and then read from the device without another
thread interfering,
you can hold on to the lock as long as needed.
```c++
std::array<std::byte, 7> dataFromDevice{};
{
    auto lockedBus = bus.lock(DEVICE_ADDRESS);
    lockedBus.write<3>({byte{0xA}, byte{0xB}, byte{0xC}});
    dataFromDevice = lockedBus.read<7>();
}
```

A common use case is first writing a register address, and then reading multiple
bytes starting at that address.
The `readFrom` function does just that:
```c++
constexpr auto REGISTER_ACCELERATION = std::byte{0x7};
auto acceleration = bus.lock(DEVICE_ADDRESS).readFrom<4>(REGISTER_ACCELERATION);
```


### Chip Driver
#### PCA9685
The library provides a driver for the `PCA9685` PWM controller:
```c++
auto pwmController = i2c::PCA9685(std::make_shared<i2c::Bus>("/dev/i2c-0"));
pwmController.setAllOff();
pwmController.setOnTime(0, 0);
pwmController.setOffTime(0, 512);
```
The On/Off time has to be determined manually currently, see the datasheet for
information on the integrated counter and prescaler.

### Board Driver
#### Servo Hat
The library provides a driver for the "SparkFun Servo pHAT", based on the [PCA9685](#pca9685)
PWM controller.
```c++
auto servoHat = i2c::ServoHat(std::make_shared<i2c::Bus>("/dev/i2c-0"));
servoHat.setServo(0, 1500);
```
This sets the servo 0 to 1500µS, which corresponds to the midpoint on most servos.
