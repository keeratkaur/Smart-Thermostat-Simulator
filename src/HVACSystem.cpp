#include "HVACSystem.h"
#include <iostream>
#include <sstream>
#include <iomanip>

HVACSystem::HVACSystem()
    : mode(HVACMode::OFF)
    , compressorStatus(CompressorStatus::OFF)
    , fanRunning(false)
    , heatRunning(false)
    , coolRunning(false)
    , efficiency(85.0)
    , powerConsumption(0.0)
    , runtime(0.0)
    , lastStartTime(std::chrono::system_clock::now())
    , lastStopTime(std::chrono::system_clock::now())
{
}

void HVACSystem::initialize() {
    std::cout << "Initializing HVAC system..." << std::endl;
    mode = HVACMode::OFF;
    compressorStatus = CompressorStatus::OFF;
    fanRunning = false;
    heatRunning = false;
    coolRunning = false;
    powerConsumption = 0.0;
    runtime = 0.0;
    lastStartTime = std::chrono::system_clock::now();
    lastStopTime = std::chrono::system_clock::now();
    std::cout << "HVAC system initialized successfully!" << std::endl;
}

void HVACSystem::update() {
    updateCompressorStatus();
    updateRuntime();
    updatePowerConsumption();
    
    // Check safety limits
    if (!checkSafetyLimits()) {
        emergencyShutdown();
    }
}

void HVACSystem::shutdown() {
    std::cout << "Shutting down HVAC system..." << std::endl;
    stopAll();
    std::cout << "HVAC system shutdown complete." << std::endl;
}

void HVACSystem::startHeat() {
    if (!canStart()) {
        std::cout << "Cannot start heating - system constraints not met" << std::endl;
        return;
    }
    
    mode = HVACMode::HEAT;
    heatRunning = true;
    coolRunning = false;
    fanRunning = true;
    
    if (compressorStatus == CompressorStatus::OFF) {
        compressorStatus = CompressorStatus::STARTING;
        lastStartTime = std::chrono::system_clock::now();
    }
    
    std::cout << "Heating system started" << std::endl;
}

void HVACSystem::startCool() {
    if (!canStart()) {
        std::cout << "Cannot start cooling - system constraints not met" << std::endl;
        return;
    }
    
    mode = HVACMode::COOL;
    coolRunning = true;
    heatRunning = false;
    fanRunning = true;
    
    if (compressorStatus == CompressorStatus::OFF) {
        compressorStatus = CompressorStatus::STARTING;
        lastStartTime = std::chrono::system_clock::now();
    }
    
    std::cout << "Cooling system started" << std::endl;
}

void HVACSystem::startFan() {
    fanRunning = true;
    std::cout << "Fan started" << std::endl;
}

void HVACSystem::stopAll() {
    if (compressorStatus == CompressorStatus::RUNNING) {
        compressorStatus = CompressorStatus::STOPPING;
        lastStopTime = std::chrono::system_clock::now();
    }
    
    heatRunning = false;
    coolRunning = false;
    fanRunning = false;
    mode = HVACMode::OFF;
    
    std::cout << "HVAC system stopped" << std::endl;
}

bool HVACSystem::isHeating() const {
    return heatRunning;
}

bool HVACSystem::isCooling() const {
    return coolRunning;
}

bool HVACSystem::isFanRunning() const {
    return fanRunning;
}

HVACMode HVACSystem::getMode() const {
    return mode;
}

CompressorStatus HVACSystem::getCompressorStatus() const {
    return compressorStatus;
}

double HVACSystem::getEfficiency() const {
    return efficiency;
}

double HVACSystem::getPowerConsumption() const {
    return powerConsumption;
}

double HVACSystem::getRuntime() const {
    return runtime;
}

std::string HVACSystem::getSystemStatus() const {
    std::stringstream ss;
    
    if (heatRunning) {
        ss << "HEATING";
    } else if (coolRunning) {
        ss << "COOLING";
    } else if (fanRunning) {
        ss << "FAN ONLY";
    } else {
        ss << "IDLE";
    }
    
    ss << " | Compressor: ";
    switch (compressorStatus) {
        case CompressorStatus::OFF: ss << "OFF"; break;
        case CompressorStatus::STARTING: ss << "STARTING"; break;
        case CompressorStatus::RUNNING: ss << "RUNNING"; break;
        case CompressorStatus::STOPPING: ss << "STOPPING"; break;
    }
    
    ss << " | Power: " << std::fixed << std::setprecision(1) << powerConsumption << " W";
    ss << " | Runtime: " << std::fixed << std::setprecision(1) << runtime << " hrs";
    
    return ss.str();
}

bool HVACSystem::canStart() const {
    auto now = std::chrono::system_clock::now();
    auto timeSinceStop = std::chrono::duration_cast<std::chrono::seconds>(now - lastStopTime);
    
    // Check minimum off time
    if (timeSinceStop.count() < MIN_OFF_TIME_SECONDS) {
        return false;
    }
    
    // Check if compressor is already running
    if (compressorStatus == CompressorStatus::RUNNING || 
        compressorStatus == CompressorStatus::STARTING) {
        return false;
    }
    
    return true;
}

bool HVACSystem::shouldStop() const {
    auto now = std::chrono::system_clock::now();
    auto runtime = std::chrono::duration_cast<std::chrono::seconds>(now - lastStartTime);
    
    // Check minimum runtime
    if (runtime.count() < MIN_RUNTIME_SECONDS) {
        return false;
    }
    
    return true;
}

void HVACSystem::emergencyShutdown() {
    std::cout << "EMERGENCY SHUTDOWN - Safety limits exceeded!" << std::endl;
    stopAll();
    compressorStatus = CompressorStatus::OFF;
}

void HVACSystem::resetRuntime() {
    runtime = 0.0;
    std::cout << "HVAC runtime reset" << std::endl;
}

void HVACSystem::setEfficiency(double newEfficiency) {
    if (newEfficiency >= 0.0 && newEfficiency <= 100.0) {
        efficiency = newEfficiency;
        std::cout << "HVAC efficiency set to " << efficiency << "%" << std::endl;
    } else {
        std::cout << "Invalid efficiency value: " << newEfficiency << "%" << std::endl;
    }
}

void HVACSystem::updateCompressorStatus() {
    auto now = std::chrono::system_clock::now();
    
    switch (compressorStatus) {
        case CompressorStatus::STARTING: {
            auto startTime = std::chrono::duration_cast<std::chrono::seconds>(now - lastStartTime);
            if (startTime.count() >= 5) { // 5 second startup time
                compressorStatus = CompressorStatus::RUNNING;
                std::cout << "Compressor started successfully" << std::endl;
            }
            break;
        }
        case CompressorStatus::STOPPING: {
            auto stopTime = std::chrono::duration_cast<std::chrono::seconds>(now - lastStopTime);
            if (stopTime.count() >= 3) { // 3 second shutdown time
                compressorStatus = CompressorStatus::OFF;
                std::cout << "Compressor stopped successfully" << std::endl;
            }
            break;
        }
        case CompressorStatus::RUNNING: {
            // Check if we should stop
            if (shouldStop() && !heatRunning && !coolRunning) {
                compressorStatus = CompressorStatus::STOPPING;
                lastStopTime = now;
            }
            break;
        }
        default:
            break;
    }
}

void HVACSystem::updateRuntime() {
    if (compressorStatus == CompressorStatus::RUNNING) {
        auto now = std::chrono::system_clock::now();
        auto runtimeDuration = std::chrono::duration_cast<std::chrono::hours>(now - lastStartTime);
        runtime = runtimeDuration.count() + (runtimeDuration.count() > 0 ? 1.0 : 0.0);
    }
}

void HVACSystem::updatePowerConsumption() {
    // Calculate power consumption based on system state
    powerConsumption = 0.0;
    
    if (fanRunning) {
        powerConsumption += 100.0; // Fan power
    }
    
    if (compressorStatus == CompressorStatus::RUNNING) {
        if (heatRunning) {
            powerConsumption += 3500.0; // Heating power
        } else if (coolRunning) {
            powerConsumption += 3000.0; // Cooling power
        }
    } else if (compressorStatus == CompressorStatus::STARTING) {
        powerConsumption += 5000.0; // Startup power surge
    }
    
    // Apply efficiency factor
    powerConsumption = powerConsumption * (100.0 / efficiency);
}

bool HVACSystem::checkSafetyLimits() const {
    // Check for excessive runtime (24 hours max)
    if (runtime > 24.0) {
        std::cout << "Safety limit exceeded: Runtime > 24 hours" << std::endl;
        return false;
    }
    
    // Check for excessive power consumption
    if (powerConsumption > 10000.0) { // 10 kW max
        std::cout << "Safety limit exceeded: Power consumption > 10 kW" << std::endl;
        return false;
    }
    
    return true;
} 