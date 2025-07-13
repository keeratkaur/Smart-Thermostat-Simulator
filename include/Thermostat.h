#pragma once

#include <memory>
#include <chrono>
#include <string>
#include "TemperatureSensor.h"
#include "HVACSystem.h"
#include "Schedule.h"
#include "WeatherSimulator.h"
#include "EnergyMonitor.h"
#include "UserInterface.h"

enum class ThermostatMode {
    HEAT,
    COOL,
    AUTO,
    OFF
};

enum class FanMode {
    AUTO,
    ON
};

class Thermostat {
private:
    std::unique_ptr<TemperatureSensor> temperatureSensor;
    std::unique_ptr<HVACSystem> hvacSystem;
    std::unique_ptr<Schedule> schedule;
    std::unique_ptr<WeatherSimulator> weatherSimulator;
    std::unique_ptr<EnergyMonitor> energyMonitor;
    std::unique_ptr<UserInterface> userInterface;

    double targetTemperature;
    double currentTemperature;
    double humidity;
    ThermostatMode mode;
    FanMode fanMode;
    bool isRunning;
    std::chrono::system_clock::time_point lastUpdate;

    // Temperature thresholds
    const double HEAT_THRESHOLD = 1.0;  // Degrees below target to start heating
    const double COOL_THRESHOLD = 1.0;  // Degrees above target to start cooling
    const double DEADBAND = 0.5;         // Temperature range to prevent rapid cycling

public:
    Thermostat();
    ~Thermostat() = default;

    // Core functionality
    void initialize();
    void run();
    void update();
    void shutdown();

    // Temperature control
    void setTargetTemperature(double temperature);
    double getTargetTemperature() const;
    double getCurrentTemperature() const;
    double getHumidity() const;

    // Mode control
    void setMode(ThermostatMode mode);
    ThermostatMode getMode() const;
    void setFanMode(FanMode mode);
    FanMode getFanMode() const;

    // Schedule management
    void setSchedule(const Schedule& newSchedule);
    Schedule* getSchedule();

    // Energy monitoring
    double getEnergyUsage() const;
    double getEnergyCost() const;
    void resetEnergyStats();

    // Weather integration
    double getOutdoorTemperature() const;
    std::string getWeatherCondition() const;

    // System status
    bool isSystemRunning() const;
    std::string getSystemStatus() const;
    std::string getDetailedStatus() const;

private:
    void processTemperatureControl();
    void updateSystemStatus();
    void logEvent(const std::string& event);
}; 