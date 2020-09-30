cmake_minimum_required(VERSION 3.13)
project(I2C VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(DEFAULT_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror -O3 -march=native -mtune=native)

find_package(Microsoft.GSL REQUIRED)

add_library(I2C src/Bus.cpp src/PCA9685.cpp src/ServoHat.cpp)
target_include_directories(I2C
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_compile_features(I2C PRIVATE cxx_std_17)
target_link_libraries(I2C PUBLIC stdc++fs Microsoft.GSL::GSL)
target_compile_options(I2C PRIVATE ${DEFAULT_COMPILE_OPTIONS})

# Run include-what-you-use while compiling if it is found on the system
find_program(IWYU include-what-you-use)
if (IWYU)
    set_target_properties(I2C PROPERTIES
            CXX_INCLUDE_WHAT_YOU_USE "include-what-you-use;-Xiwyu;--no_fwd_decls;-Xiwyu;--cxx17ns;-Xiwyu;--quoted_includes_first")
endif ()

# Run clang-tidy while compiling if it is found on the system
find_program(CLANG-TIDY clang-tidy REQUIRED)
if (CLANG-TIDY)
    set_target_properties(I2C PROPERTIES
            CXX_CLANG_TIDY "clang-tidy")
endif ()

set_target_properties(I2C PROPERTIES CXX_LINK_WHAT_YOU_USE TRUE)

# Install
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/ottojo.I2C)


install(TARGETS I2C
        EXPORT i2c-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT i2c-targets
        FILE
        ottojo.I2CTargets.cmake
        NAMESPACE
        ottojo.I2C::
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/ottojo.I2C
        )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/ottojo.I2CConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ottojo.I2CConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/ottojo.I2CConfig.cmake
        INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
        )

#Install the config, configversion and custom find modules
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ottojo.I2CConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ottojo.I2CConfigVersion.cmake
        DESTINATION ${INSTALL_CONFIGDIR}
        )

export(EXPORT i2c-targets
        FILE ${CMAKE_CURRENT_BINARY_DIR}/ottojo.I2CTargets.cmake
        NAMESPACE ottojo.I2C::)

export(PACKAGE ottojo.I2C)

add_subdirectory(test)
