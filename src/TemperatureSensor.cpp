#include "TemperatureSensor.h"
#include <iostream>
#include <cmath>

TemperatureSensor::TemperatureSensor(double initialTemp, double initialHumidity)
    : rng(std::random_device{}())
    , tempVariation(-0.5, 0.5)
    , humidityVariation(-2.0, 2.0)
    , baseTemperature(initialTemp)
    , baseHumidity(initialHumidity)
    , currentTemperature(initialTemp)
    , currentHumidity(initialHumidity)
    , lastUpdate(std::chrono::system_clock::now())
{
}

void TemperatureSensor::initialize() {
    std::cout << "Initializing temperature sensor..." << std::endl;
    lastUpdate = std::chrono::system_clock::now();
    std::cout << "Temperature sensor initialized. Base temp: " << baseTemperature << "°C, Base humidity: " << baseHumidity << "%" << std::endl;
}

void TemperatureSensor::update() {
    if (shouldUpdate()) {
        generateRandomVariation();
        lastUpdate = std::chrono::system_clock::now();
    }
}

void TemperatureSensor::shutdown() {
    std::cout << "Temperature sensor shutdown." << std::endl;
}

double TemperatureSensor::getTemperature() const {
    return currentTemperature;
}

double TemperatureSensor::getHumidity() const {
    return currentHumidity;
}

void TemperatureSensor::calibrate(double referenceTemperature) {
    double offset = referenceTemperature - currentTemperature;
    baseTemperature += offset;
    currentTemperature = referenceTemperature;
    std::cout << "Temperature sensor calibrated. Offset: " << offset << "°C" << std::endl;
}

void TemperatureSensor::setBaseTemperature(double temperature) {
    baseTemperature = temperature;
    currentTemperature = temperature;
}

void TemperatureSensor::setBaseHumidity(double humidity) {
    baseHumidity = humidity;
    currentHumidity = humidity;
}

bool TemperatureSensor::isOperational() const {
    return currentTemperature >= -50.0 && currentTemperature <= 100.0 &&
           currentHumidity >= 0.0 && currentHumidity <= 100.0;
}

std::string TemperatureSensor::getSensorStatus() const {
    if (isOperational()) {
        return "OPERATIONAL";
    } else {
        return "ERROR - Sensor readings out of range";
    }
}

void TemperatureSensor::simulateTemperatureChange(double targetTemp, double ratePerMinute) {
    double currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - lastUpdate).count() / 1000.0 / 60.0; // Convert to minutes
    
    double change = ratePerMinute * currentTime;
    currentTemperature = baseTemperature + change;
    
    // Add some random variation
    currentTemperature += tempVariation(rng);
    
    // Clamp to reasonable range
    currentTemperature = std::max(-50.0, std::min(100.0, currentTemperature));
}

void TemperatureSensor::simulateHumidityChange(double targetHumidity, double ratePerMinute) {
    double currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now() - lastUpdate).count() / 1000.0 / 60.0; // Convert to minutes
    
    double change = ratePerMinute * currentTime;
    currentHumidity = baseHumidity + change;
    
    // Add some random variation
    currentHumidity += humidityVariation(rng);
    
    // Clamp to reasonable range
    currentHumidity = std::max(0.0, std::min(100.0, currentHumidity));
}

void TemperatureSensor::generateRandomVariation() {
    // Add small random variations to simulate real sensor behavior
    double tempVariation = this->tempVariation(rng);
    double humidityVariation = this->humidityVariation(rng);
    
    currentTemperature += tempVariation * 0.1; // Small variation
    currentHumidity += humidityVariation * 0.1;
    
    // Clamp values to reasonable ranges
    currentTemperature = std::max(-50.0, std::min(100.0, currentTemperature));
    currentHumidity = std::max(0.0, std::min(100.0, currentHumidity));
}

bool TemperatureSensor::shouldUpdate() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
    return duration.count() >= UPDATE_INTERVAL_MS;
} 