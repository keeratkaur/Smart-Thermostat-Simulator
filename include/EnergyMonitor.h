#pragma once

#include <vector>
#include <string>
#include <chrono>

struct EnergyUsage {
    double powerConsumption;  // Watts
    double energyUsed;        // kWh
    double cost;              // Dollars
    std::chrono::system_clock::time_point timestamp;
};

struct DailyEnergyReport {
    double totalEnergy;
    double totalCost;
    double peakPower;
    double averagePower;
    int runtimeHours;
    int runtimeMinutes;
    std::vector<EnergyUsage> hourlyData;
};

class EnergyMonitor {
private:
    std::vector<EnergyUsage> usageHistory;
    std::vector<DailyEnergyReport> dailyReports;
    
    double currentPowerConsumption;
    double totalEnergyUsed;
    double totalCost;
    double costPerKWh;
    
    std::chrono::system_clock::time_point lastUpdate;
    std::chrono::system_clock::time_point dayStart;
    
    // Energy efficiency metrics
    double efficiencyRating;
    double carbonFootprint;
    double moneySaved;
    
    // Historical data limits
    const size_t MAX_HISTORY_SIZE = 1000;
    const size_t MAX_DAILY_REPORTS = 30;

public:
    EnergyMonitor(double costPerKWh = 0.12);
    ~EnergyMonitor() = default;

    // Core functionality
    void initialize();
    void update(double currentPower);
    void shutdown();
    
    // Energy tracking
    void recordEnergyUsage(double power, double energy, double cost);
    void updatePowerConsumption(double power);
    void calculateCost();
    
    // Energy reports
    double getCurrentPowerConsumption() const;
    double getTotalEnergyUsed() const;
    double getTotalCost() const;
    double getDailyEnergyUsage() const;
    double getDailyCost() const;
    
    // Efficiency analysis
    double getEfficiencyRating() const;
    double getCarbonFootprint() const;
    double getMoneySaved() const;
    std::string getEfficiencyRecommendation() const;
    
    // Historical data
    std::vector<EnergyUsage> getUsageHistory() const;
    std::vector<DailyEnergyReport> getDailyReports() const;
    DailyEnergyReport getTodayReport() const;
    DailyEnergyReport getYesterdayReport() const;
    
    // Cost management
    void setCostPerKWh(double cost);
    double getCostPerKWh() const;
    double estimateMonthlyCost() const;
    double estimateYearlyCost() const;
    
    // Data management
    void resetDailyStats();
    void resetAllStats();
    void generateDailyReport();
    void cleanupOldData();

private:
    void updateEfficiencyMetrics();
    void updateCarbonFootprint();
    void updateMoneySaved();
    bool shouldGenerateDailyReport() const;
    void addToHistory(const EnergyUsage& usage);
}; 