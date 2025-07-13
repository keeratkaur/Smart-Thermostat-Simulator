# Smart Thermostat Simulator

A comprehensive C++ simulation of a smart thermostat system with realistic HVAC control, weather integration, energy monitoring, and user interface.

## Features

### Core Functionality
- **Temperature & Humidity Monitoring**: Realistic sensor simulation with calibration and drift
- **Smart HVAC Control**: Heating, cooling, and fan control with safety limits
- **Programmable Schedules**: Weekly temperature schedules with automatic execution
- **Weather Integration**: Outdoor weather simulation with seasonal variations
- **Energy Monitoring**: Real-time power consumption tracking and cost analysis
- **User Interface**: Simulated display with multiple screens and menu system

### Advanced Features
- **Safety Systems**: Compressor protection, runtime limits, and emergency shutdown
- **Energy Efficiency**: Carbon footprint calculation and money savings tracking
- **Weather Alerts**: Extreme weather condition detection and alerts
- **Schedule Management**: Flexible weekly programming with validation
- **System Diagnostics**: Comprehensive status reporting and troubleshooting

## Project Structure

```
Smart Thermostat Simulator/
├── CMakeLists.txt          # CMake build configuration
├── README.md              # This file
├── include/               # Header files
│   ├── Thermostat.h       # Main thermostat controller
│   ├── TemperatureSensor.h # Temperature and humidity sensor
│   ├── HVACSystem.h       # Heating, ventilation, air conditioning
│   ├── Schedule.h         # Temperature scheduling system
│   ├── WeatherSimulator.h # Outdoor weather simulation
│   ├── EnergyMonitor.h    # Energy consumption tracking
│   └── UserInterface.h    # User interface and display
├── src/                   # Source files
│   ├── main.cpp           # Application entry point
│   ├── Thermostat.cpp     # Main thermostat implementation
│   ├── TemperatureSensor.cpp
│   ├── HVACSystem.cpp
│   ├── Schedule.cpp
│   ├── WeatherSimulator.cpp
│   ├── EnergyMonitor.cpp
│   └── UserInterface.cpp
└── build/                 # Build output directory (created)
```

## Building the Project

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16 or higher
- Make or Ninja build system

### Build Instructions

#### Linux/macOS
```bash
# Clone or navigate to the project directory
cd Smart\ Thermostat\ Simulator

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make

# Run the simulator
./SmartThermostatSimulator
```

#### Windows (Visual Studio)
```bash
# Create build directory
mkdir build
cd build

# Configure with Visual Studio
cmake .. -G "Visual Studio 16 2019"

# Build
cmake --build . --config Release

# Run the simulator
.\Release\SmartThermostatSimulator.exe
```

#### Windows (MinGW)
```bash
# Create build directory
mkdir build
cd build

# Configure with MinGW
cmake .. -G "MinGW Makefiles"

# Build
mingw32-make

# Run the simulator
./SmartThermostatSimulator.exe
```

## Usage

### Command Line Options
```bash
./SmartThermostatSimulator [options]

Options:
  --help, -h          Show help message
  --verbose, -v       Enable verbose output
  --demo, -d          Run in demo mode with preset values
  --config <file>     Load configuration from file (future feature)
```

### Examples
```bash
# Run with default settings
./SmartThermostatSimulator

# Run with verbose output
./SmartThermostatSimulator --verbose

# Run in demo mode
./SmartThermostatSimulator --demo

# Run with both verbose and demo
./SmartThermostatSimulator -v -d
```

## Simulation Features

### Temperature Control
- **Target Temperature**: 10-30°C range
- **Deadband**: 0.5°C to prevent rapid cycling
- **Heat/Cool Thresholds**: 1.0°C above/below target
- **Auto Mode**: Automatically switches between heating and cooling

### HVAC System
- **Compressor Protection**: Minimum runtime (5 min) and off-time (1 min)
- **Power Monitoring**: Real-time power consumption tracking
- **Safety Limits**: Maximum runtime (24 hours) and power (10 kW)
- **Efficiency Rating**: 85% base efficiency with dynamic adjustment

### Weather Simulation
- **Seasonal Variations**: Temperature changes based on current month
- **Weather Conditions**: Sunny, Cloudy, Rainy, Snowy, Windy, Stormy
- **Heat/Cooling Load**: Calculated based on outdoor conditions
- **Weather Alerts**: Extreme temperature and wind conditions

### Energy Monitoring
- **Real-time Tracking**: Power consumption and energy usage
- **Cost Calculation**: Based on configurable rate per kWh
- **Efficiency Metrics**: Carbon footprint and money savings
- **Historical Data**: Daily reports and usage history

### Schedule System
- **Weekly Programming**: 7-day schedule with multiple entries per day
- **Automatic Execution**: Temperature changes based on time
- **Validation**: Time and temperature range checking
- **Default Schedule**: Typical weekday/weekend patterns

## System Architecture

### Component Interaction
```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   UserInterface │    │  Temperature    │    │   HVACSystem    │
│                 │    │    Sensor       │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│    Thermostat   │◄──►│     Schedule    │    │  EnergyMonitor  │
│   (Controller)  │    │                 │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ WeatherSimulator│    │   Temperature   │    │   Power Usage   │
│                 │    │   Drift Logic   │    │   Calculation   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

### Update Cycle
1. **Sensor Reading**: Temperature and humidity measurement
2. **Weather Update**: Outdoor conditions and forecast
3. **Schedule Check**: Current target temperature from schedule
4. **Control Logic**: Determine heating/cooling needs
5. **HVAC Control**: Start/stop heating/cooling systems
6. **Energy Tracking**: Monitor power consumption
7. **User Interface**: Update display and handle input
8. **System Status**: Log events and update diagnostics

## Configuration

### Default Settings
- **Target Temperature**: 22.0°C
- **Mode**: AUTO (automatic heating/cooling)
- **Fan Mode**: AUTO (fan runs with HVAC)
- **Energy Cost**: $0.12 per kWh
- **Update Interval**: 1 second
- **Display Timeout**: 30 seconds

### Schedule Defaults
- **Weekdays**: 6:00 (20°C), 8:00 (18°C), 17:00 (20°C), 22:00 (18°C)
- **Weekends**: 7:00 (21°C), 23:00 (19°C)

## Troubleshooting

### Common Issues

#### Build Errors
- **CMake not found**: Install CMake 3.16 or higher
- **Compiler not found**: Install a C++17 compatible compiler
- **Missing headers**: Ensure all include files are present

#### Runtime Issues
- **Permission denied**: Check file permissions on executable
- **Missing libraries**: Ensure all dependencies are installed
- **Memory issues**: Check system resources

### Debug Mode
Run with verbose output to see detailed system information:
```bash
./SmartThermostatSimulator --verbose
```

## Future Enhancements

### Planned Features
- **Configuration Files**: JSON-based settings
- **Network Interface**: Remote control and monitoring
- **Data Logging**: Persistent storage of system data
- **Advanced Scheduling**: Holiday and vacation modes
- **Mobile App**: Remote thermostat control
- **Machine Learning**: Predictive temperature control

### Technical Improvements
- **Multi-threading**: Parallel component updates
- **Real-time OS**: RTOS integration for timing
- **Hardware Interface**: Actual sensor and actuator control
- **Web Interface**: HTTP-based control interface
- **Database**: SQLite for data persistence

## Contributing

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

### Code Style
- Follow C++17 standards
- Use meaningful variable and function names
- Add comments for complex logic
- Maintain consistent indentation
- Include error handling

## License

This project is provided as educational software for learning C++ programming concepts and embedded systems simulation.

## Acknowledgments

- Inspired by modern smart thermostat systems
- Designed for educational and demonstration purposes
- Demonstrates object-oriented design patterns
- Shows real-time system simulation techniques

---

**Note**: This is a simulation project for educational purposes. It does not control actual HVAC systems and should not be used for real home automation without proper safety considerations. 
