#!/bin/bash

# C++ Smart Thermostat Simulator Build Script
# This script builds the project using CMake

set -e  # Exit on any error

echo "==============================================="
echo "    C++ Smart Thermostat Simulator Build"
echo "==============================================="

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed. Please install CMake 3.16 or higher."
    exit 1
fi

# Check CMake version
CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
echo "Found CMake version: $CMAKE_VERSION"

# Check if C++ compiler is available
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo "Error: No C++ compiler found. Please install GCC or Clang."
    exit 1
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Configure the project
echo "Configuring project with CMake..."
cmake ..

# Build the project
echo "Building project..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# Check if build was successful
if [ -f "SmartThermostatSimulator" ]; then
    echo ""
    echo "==============================================="
    echo "Build completed successfully!"
    echo "==============================================="
    echo ""
    echo "To run the simulator:"
    echo "  ./SmartThermostatSimulator"
    echo ""
    echo "To run with verbose output:"
    echo "  ./SmartThermostatSimulator --verbose"
    echo ""
    echo "To run in demo mode:"
    echo "  ./SmartThermostatSimulator --demo"
    echo ""
    echo "For help:"
    echo "  ./SmartThermostatSimulator --help"
    echo ""
else
    echo "Error: Build failed. Executable not found."
    exit 1
fi 