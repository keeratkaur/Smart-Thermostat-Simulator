#pragma once

#include <string>
#include <chrono>

enum class HVACMode {
    HEAT,
    COOL,
    FAN_ONLY,
    OFF
};

enum class CompressorStatus {
    OFF,
    STARTING,
    RUNNING,
    STOPPING
};

class HVACSystem {
private:
    HVACMode mode;
    CompressorStatus compressorStatus;
    bool fanRunning;
    bool heatRunning;
    bool coolRunning;
    
    double efficiency;
    double powerConsumption;
    double runtime;
    
    std::chrono::system_clock::time_point lastStartTime;
    std::chrono::system_clock::time_point lastStopTime;
    
    // System limits
    const int MIN_RUNTIME_SECONDS = 300;  // 5 minutes minimum runtime
    const int MIN_OFF_TIME_SECONDS = 60;   // 1 minute minimum off time
    const double MAX_TEMP = 30.0;          // Maximum safe temperature
    const double MIN_TEMP = 10.0;          // Minimum safe temperature

public:
    HVACSystem();
    ~HVACSystem() = default;

    // Core functionality
    void initialize();
    void update();
    void shutdown();

    // System control
    void startHeat();
    void startCool();
    void startFan();
    void stopAll();
    
    // Status queries
    bool isHeating() const;
    bool isCooling() const;
    bool isFanRunning() const;
    HVACMode getMode() const;
    CompressorStatus getCompressorStatus() const;
    
    // Performance monitoring
    double getEfficiency() const;
    double getPowerConsumption() const;
    double getRuntime() const;
    std::string getSystemStatus() const;
    
    // Safety and limits
    bool canStart() const;
    bool shouldStop() const;
    void emergencyShutdown();
    
    // Maintenance
    void resetRuntime();
    void setEfficiency(double newEfficiency);

private:
    void updateCompressorStatus();
    void updateRuntime();
    void updatePowerConsumption();
    bool checkSafetyLimits() const;
}; 