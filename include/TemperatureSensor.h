#pragma once

#include <random>
#include <chrono>

class TemperatureSensor {
private:
    std::mt19937 rng;
    std::uniform_real_distribution<double> tempVariation;
    std::uniform_real_distribution<double> humidityVariation;
    
    double baseTemperature;
    double baseHumidity;
    double currentTemperature;
    double currentHumidity;
    
    std::chrono::system_clock::time_point lastUpdate;
    const int UPDATE_INTERVAL_MS = 1000; // Update every second

public:
    TemperatureSensor(double initialTemp = 22.0, double initialHumidity = 45.0);
    ~TemperatureSensor() = default;

    // Core functionality
    void initialize();
    void update();
    void shutdown();

    // Temperature readings
    double getTemperature() const;
    double getHumidity() const;
    
    // Sensor calibration
    void calibrate(double referenceTemperature);
    void setBaseTemperature(double temperature);
    void setBaseHumidity(double humidity);
    
    // Sensor status
    bool isOperational() const;
    std::string getSensorStatus() const;
    
    // Environmental simulation
    void simulateTemperatureChange(double targetTemp, double ratePerMinute);
    void simulateHumidityChange(double targetHumidity, double ratePerMinute);

private:
    void generateRandomVariation();
    bool shouldUpdate() const;
}; 