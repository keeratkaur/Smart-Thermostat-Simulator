@echo off
REM C++ Smart Thermostat Simulator Build Script for Windows
REM This script builds the project using CMake

echo ===============================================
echo     C++ Smart Thermostat Simulator Build
echo ===============================================

REM Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
    echo Error: CMake is not installed. Please install CMake 3.16 or higher.
    echo Download from: https://cmake.org/download/
    pause
    exit /b 1
)

REM Check CMake version
for /f "tokens=3" %%i in ('cmake --version 2^>nul ^| findstr /r "cmake version"') do set CMAKE_VERSION=%%i
echo Found CMake version: %CMAKE_VERSION%

REM Check if Visual Studio is available
where cl >nul 2>&1
if errorlevel 1 (
    echo Warning: Visual Studio compiler not found in PATH.
    echo Trying to use MinGW or other available compiler...
)

REM Create build directory
echo Creating build directory...
if not exist build mkdir build
cd build

REM Configure the project
echo Configuring project with CMake...
cmake .. -G "Visual Studio 16 2019" -A x64
if errorlevel 1 (
    echo Trying with MinGW...
    cmake .. -G "MinGW Makefiles"
    if errorlevel 1 (
        echo Error: Failed to configure project with CMake.
        pause
        exit /b 1
    )
)

REM Build the project
echo Building project...
if exist CMakeCache.txt (
    findstr /C:"CMAKE_GENERATOR:INTERNAL=Visual Studio" CMakeCache.txt >nul
    if errorlevel 1 (
        REM MinGW build
        mingw32-make
    ) else (
        REM Visual Studio build
        cmake --build . --config Release
    )
) else (
    echo Error: CMake configuration failed.
    pause
    exit /b 1
)

REM Check if build was successful
if exist "SmartThermostatSimulator.exe" (
    echo.
    echo ===============================================
    echo Build completed successfully!
    echo ===============================================
    echo.
    echo To run the simulator:
    echo   SmartThermostatSimulator.exe
    echo.
    echo To run with verbose output:
    echo   SmartThermostatSimulator.exe --verbose
    echo.
    echo To run in demo mode:
    echo   SmartThermostatSimulator.exe --demo
    echo.
    echo For help:
    echo   SmartThermostatSimulator.exe --help
    echo.
) else if exist "Release\SmartThermostatSimulator.exe" (
    echo.
    echo ===============================================
    echo Build completed successfully!
    echo ===============================================
    echo.
    echo To run the simulator:
    echo   Release\SmartThermostatSimulator.exe
    echo.
    echo To run with verbose output:
    echo   Release\SmartThermostatSimulator.exe --verbose
    echo.
    echo To run in demo mode:
    echo   Release\SmartThermostatSimulator.exe --demo
    echo.
    echo For help:
    echo   Release\SmartThermostatSimulator.exe --help
    echo.
) else (
    echo Error: Build failed. Executable not found.
    pause
    exit /b 1
)

pause 