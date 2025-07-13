#include "Thermostat.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Thermostat::Thermostat()
    : targetTemperature(22.0)
    , currentTemperature(22.0)
    , humidity(45.0)
    , mode(ThermostatMode::AUTO)
    , fanMode(FanMode::AUTO)
    , isRunning(false)
    , lastUpdate(std::chrono::system_clock::now())
{
    // Initialize component pointers
    temperatureSensor = std::make_unique<TemperatureSensor>();
    hvacSystem = std::make_unique<HVACSystem>();
    schedule = std::make_unique<Schedule>();
    weatherSimulator = std::make_unique<WeatherSimulator>();
    energyMonitor = std::make_unique<EnergyMonitor>();
    userInterface = std::make_unique<UserInterface>();
}

void Thermostat::initialize() {
    std::cout << "Initializing Smart Thermostat System..." << std::endl;
    
    // Initialize all components
    temperatureSensor->initialize();
    hvacSystem->initialize();
    schedule->initialize();
    weatherSimulator->initialize();
    energyMonitor->initialize();
    userInterface->initialize();
    
    isRunning = true;
    lastUpdate = std::chrono::system_clock::now();
    
    std::cout << "Smart Thermostat System initialized successfully!" << std::endl;
}

void Thermostat::run() {
    if (!isRunning) {
        std::cout << "Thermostat is not running. Call initialize() first." << std::endl;
        return;
    }
    
    while (isRunning) {
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Thermostat::update() {
    auto now = std::chrono::system_clock::now();
    
    // Update all components
    temperatureSensor->update();
    hvacSystem->update();
    schedule->loadDefaultSchedule();
    weatherSimulator->update();
    energyMonitor->update(hvacSystem->getPowerConsumption());
    userInterface->update();
    
    // Update current readings
    currentTemperature = temperatureSensor->getTemperature();
    humidity = temperatureSensor->getHumidity();
    
    // Process temperature control logic
    processTemperatureControl();
    
    // Update system status
    updateSystemStatus();
    
    lastUpdate = now;
}

void Thermostat::shutdown() {
    std::cout << "Shutting down Smart Thermostat System..." << std::endl;
    
    isRunning = false;
    
    // Shutdown all components
    temperatureSensor->shutdown();
    hvacSystem->shutdown();
    weatherSimulator->shutdown();
    energyMonitor->shutdown();
    userInterface->shutdown();
    
    std::cout << "Smart Thermostat System shutdown complete." << std::endl;
}

void Thermostat::setTargetTemperature(double temperature) {
    if (temperature >= 10.0 && temperature <= 30.0) {
        targetTemperature = temperature;
        logEvent("Target temperature set to " + std::to_string(temperature) + "°C");
    } else {
        std::cout << "Invalid temperature: " << temperature << "°C (must be between 10-30°C)" << std::endl;
    }
}

double Thermostat::getTargetTemperature() const {
    return targetTemperature;
}

double Thermostat::getCurrentTemperature() const {
    return currentTemperature;
}

double Thermostat::getHumidity() const {
    return humidity;
}

void Thermostat::setMode(ThermostatMode newMode) {
    mode = newMode;
    std::string modeStr;
    switch (mode) {
        case ThermostatMode::HEAT: modeStr = "HEAT"; break;
        case ThermostatMode::COOL: modeStr = "COOL"; break;
        case ThermostatMode::AUTO: modeStr = "AUTO"; break;
        case ThermostatMode::OFF: modeStr = "OFF"; break;
    }
    logEvent("Mode changed to " + modeStr);
}

ThermostatMode Thermostat::getMode() const {
    return mode;
}

void Thermostat::setFanMode(FanMode newFanMode) {
    fanMode = newFanMode;
    std::string fanModeStr = (fanMode == FanMode::AUTO) ? "AUTO" : "ON";
    logEvent("Fan mode changed to " + fanModeStr);
}

FanMode Thermostat::getFanMode() const {
    return fanMode;
}

void Thermostat::setSchedule(const Schedule& newSchedule) {
    *schedule = newSchedule;
    logEvent("Schedule updated");
}

Schedule* Thermostat::getSchedule() {
    return schedule.get();
}

double Thermostat::getEnergyUsage() const {
    return energyMonitor->getTotalEnergyUsed();
}

double Thermostat::getEnergyCost() const {
    return energyMonitor->getTotalCost();
}

void Thermostat::resetEnergyStats() {
    energyMonitor->resetAllStats();
    logEvent("Energy statistics reset");
}

double Thermostat::getOutdoorTemperature() const {
    return weatherSimulator->getOutdoorTemperature();
}

std::string Thermostat::getWeatherCondition() const {
    return weatherSimulator->getWeatherDescription();
}

bool Thermostat::isSystemRunning() const {
    return isRunning;
}

std::string Thermostat::getSystemStatus() const {
    std::stringstream ss;
    ss << "Current: " << std::fixed << std::setprecision(1) << currentTemperature << "°C";
    ss << " | Target: " << targetTemperature << "°C";
    ss << " | Humidity: " << humidity << "%";
    
    std::string modeStr;
    switch (mode) {
        case ThermostatMode::HEAT: modeStr = "HEAT"; break;
        case ThermostatMode::COOL: modeStr = "COOL"; break;
        case ThermostatMode::AUTO: modeStr = "AUTO"; break;
        case ThermostatMode::OFF: modeStr = "OFF"; break;
    }
    ss << " | Mode: " << modeStr;
    
    return ss.str();
}

std::string Thermostat::getDetailedStatus() const {
    std::stringstream ss;
    
    // Basic temperature info
    ss << "=== THERMOSTAT STATUS ===" << std::endl;
    ss << "Current Temperature: " << std::fixed << std::setprecision(1) << currentTemperature << "°C" << std::endl;
    ss << "Target Temperature: " << targetTemperature << "°C" << std::endl;
    ss << "Humidity: " << humidity << "%" << std::endl;
    ss << "Temperature Difference: " << (currentTemperature - targetTemperature) << "°C" << std::endl;
    
    // Mode information
    std::string modeStr;
    switch (mode) {
        case ThermostatMode::HEAT: modeStr = "HEAT"; break;
        case ThermostatMode::COOL: modeStr = "COOL"; break;
        case ThermostatMode::AUTO: modeStr = "AUTO"; break;
        case ThermostatMode::OFF: modeStr = "OFF"; break;
    }
    ss << "Mode: " << modeStr << std::endl;
    
    // HVAC status
    ss << std::endl << "=== HVAC SYSTEM ===" << std::endl;
    ss << "System Status: " << hvacSystem->getSystemStatus() << std::endl;
    ss << "Heating: " << (hvacSystem->isHeating() ? "ON" : "OFF") << std::endl;
    ss << "Cooling: " << (hvacSystem->isCooling() ? "ON" : "OFF") << std::endl;
    ss << "Fan: " << (hvacSystem->isFanRunning() ? "ON" : "OFF") << std::endl;
    ss << "Efficiency: " << std::fixed << std::setprecision(1) << hvacSystem->getEfficiency() << "%" << std::endl;
    
    // Weather information
    ss << std::endl << "=== WEATHER ===" << std::endl;
    ss << "Outdoor Temperature: " << weatherSimulator->getOutdoorTemperature() << "°C" << std::endl;
    ss << "Weather Condition: " << weatherSimulator->getWeatherDescription() << std::endl;
    ss << "Heat Load: " << weatherSimulator->getHeatLoad() << " kW" << std::endl;
    ss << "Cooling Load: " << weatherSimulator->getCoolingLoad() << " kW" << std::endl;
    
    // Energy information
    ss << std::endl << "=== ENERGY USAGE ===" << std::endl;
    ss << "Current Power: " << std::fixed << std::setprecision(2) << energyMonitor->getCurrentPowerConsumption() << " W" << std::endl;
    ss << "Total Energy: " << energyMonitor->getTotalEnergyUsed() << " kWh" << std::endl;
    ss << "Total Cost: $" << std::fixed << std::setprecision(2) << energyMonitor->getTotalCost() << std::endl;
    ss << "Daily Usage: " << energyMonitor->getDailyEnergyUsage() << " kWh" << std::endl;
    ss << "Daily Cost: $" << energyMonitor->getDailyCost() << std::endl;
    
    // Schedule information
    ss << std::endl << "=== SCHEDULE ===" << std::endl;
    ss << "Schedule Enabled: " << (schedule->isScheduleEnabled() ? "YES" : "NO") << std::endl;
    ss << "Next Change: " << schedule->getNextScheduleChange() << std::endl;
    
    return ss.str();
}

void Thermostat::processTemperatureControl() {
    if (mode == ThermostatMode::OFF) {
        hvacSystem->stopAll();
        return;
    }
    
    double tempDiff = currentTemperature - targetTemperature;
    
    // Auto mode logic
    if (mode == ThermostatMode::AUTO) {
        if (tempDiff < -HEAT_THRESHOLD) {
            // Need heating
            if (hvacSystem->canStart()) {
                hvacSystem->startHeat();
                logEvent("Heating started - temperature below target");
            }
        } else if (tempDiff > COOL_THRESHOLD) {
            // Need cooling
            if (hvacSystem->canStart()) {
                hvacSystem->startCool();
                logEvent("Cooling started - temperature above target");
            }
        } else if (std::abs(tempDiff) < DEADBAND) {
            // Within deadband - stop HVAC
            if (hvacSystem->isHeating() || hvacSystem->isCooling()) {
                hvacSystem->stopAll();
                logEvent("HVAC stopped - temperature within deadband");
            }
        }
    }
    // Heat mode logic
    else if (mode == ThermostatMode::HEAT) {
        if (tempDiff < -HEAT_THRESHOLD) {
            if (hvacSystem->canStart()) {
                hvacSystem->startHeat();
            }
        } else if (tempDiff > -DEADBAND) {
            if (hvacSystem->isHeating()) {
                hvacSystem->stopAll();
            }
        }
    }
    // Cool mode logic
    else if (mode == ThermostatMode::COOL) {
        if (tempDiff > COOL_THRESHOLD) {
            if (hvacSystem->canStart()) {
                hvacSystem->startCool();
            }
        } else if (tempDiff < DEADBAND) {
            if (hvacSystem->isCooling()) {
                hvacSystem->stopAll();
            }
        }
    }
    
    // Fan control
    if (fanMode == FanMode::ON || (fanMode == FanMode::AUTO && (hvacSystem->isHeating() || hvacSystem->isCooling()))) {
        hvacSystem->startFan();
    } else {
        // Fan will be controlled by HVAC system in auto mode
    }
}

void Thermostat::updateSystemStatus() {
    // Update temperature sensor based on HVAC operation
    if (hvacSystem->isHeating()) {
        temperatureSensor->simulateTemperatureChange(targetTemperature, 2.0); // 2°C per minute
    } else if (hvacSystem->isCooling()) {
        temperatureSensor->simulateTemperatureChange(targetTemperature, -2.0); // -2°C per minute
    }
    
    // Update weather impact
    double outdoorTemp = weatherSimulator->getOutdoorTemperature();
    double tempDiff = outdoorTemp - currentTemperature;
    
    // Natural temperature drift based on outdoor temperature
    if (!hvacSystem->isHeating() && !hvacSystem->isCooling()) {
        double driftRate = tempDiff * 0.01; // Slow drift towards outdoor temperature
        temperatureSensor->simulateTemperatureChange(currentTemperature + driftRate, driftRate * 60);
    }
}

void Thermostat::logEvent(const std::string& event) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string timeStr = std::ctime(&now);
    timeStr.pop_back(); // Remove newline
    
    std::cout << "[" << timeStr << "] " << event << std::endl;
} 