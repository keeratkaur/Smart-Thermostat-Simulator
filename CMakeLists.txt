cmake_minimum_required(VERSION 3.16)
project(SmartThermostatSimulator VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/Thermostat.cpp
    src/TemperatureSensor.cpp
    src/HVACSystem.cpp
    src/Schedule.cpp
    src/WeatherSimulator.cpp
    src/EnergyMonitor.cpp
    src/UserInterface.cpp
)

# Header files
set(HEADERS
    include/Thermostat.h
    include/TemperatureSensor.h
    include/HVACSystem.h
    include/Schedule.h
    include/WeatherSimulator.h
    include/EnergyMonitor.h
    include/UserInterface.h
)

# Create executable
add_executable(SmartThermostatSimulator ${SOURCES} ${HEADERS})

# Set output directory
set_target_properties(SmartThermostatSimulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install target
install(TARGETS SmartThermostatSimulator
    RUNTIME DESTINATION bin
) 