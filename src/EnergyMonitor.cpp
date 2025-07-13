#include "EnergyMonitor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

EnergyMonitor::EnergyMonitor(double costPerKWh)
    : currentPowerConsumption(0.0)
    , totalEnergyUsed(0.0)
    , totalCost(0.0)
    , costPerKWh(costPerKWh)
    , efficiencyRating(85.0)
    , carbonFootprint(0.0)
    , moneySaved(0.0)
    , lastUpdate(std::chrono::system_clock::now())
    , dayStart(std::chrono::system_clock::now())
{
}

void EnergyMonitor::initialize() {
    std::cout << "Initializing energy monitor..." << std::endl;
    currentPowerConsumption = 0.0;
    totalEnergyUsed = 0.0;
    totalCost = 0.0;
    efficiencyRating = 85.0;
    carbonFootprint = 0.0;
    moneySaved = 0.0;
    lastUpdate = std::chrono::system_clock::now();
    dayStart = std::chrono::system_clock::now();
    std::cout << "Energy monitor initialized successfully!" << std::endl;
}

void EnergyMonitor::update(double currentPower) {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
    double timeHours = duration.count() / 1000.0 / 3600.0; // Convert to hours
    
    // Update current power consumption
    currentPowerConsumption = currentPower;
    
    // Calculate energy used since last update
    double energyUsed = currentPower * timeHours / 1000.0; // Convert to kWh
    double cost = energyUsed * costPerKWh;
    
    // Record energy usage
    recordEnergyUsage(currentPower, energyUsed, cost);
    
    // Update totals
    totalEnergyUsed += energyUsed;
    totalCost += cost;
    
    // Update efficiency metrics
    updateEfficiencyMetrics();
    updateCarbonFootprint();
    updateMoneySaved();
    
    // Check if we need to generate daily report
    if (shouldGenerateDailyReport()) {
        generateDailyReport();
    }
    
    lastUpdate = now;
}

void EnergyMonitor::shutdown() {
    std::cout << "Shutting down energy monitor..." << std::endl;
    generateDailyReport();
    std::cout << "Energy monitor shutdown complete." << std::endl;
}

void EnergyMonitor::recordEnergyUsage(double power, double energy, double cost) {
    EnergyUsage usage;
    usage.powerConsumption = power;
    usage.energyUsed = energy;
    usage.cost = cost;
    usage.timestamp = std::chrono::system_clock::now();
    
    addToHistory(usage);
}

void EnergyMonitor::updatePowerConsumption(double power) {
    currentPowerConsumption = power;
}

void EnergyMonitor::calculateCost() {
    totalCost = totalEnergyUsed * costPerKWh;
}

double EnergyMonitor::getCurrentPowerConsumption() const {
    return currentPowerConsumption;
}

double EnergyMonitor::getTotalEnergyUsed() const {
    return totalEnergyUsed;
}

double EnergyMonitor::getTotalCost() const {
    return totalCost;
}

double EnergyMonitor::getDailyEnergyUsage() const {
    auto now = std::chrono::system_clock::now();
    auto dayDuration = std::chrono::duration_cast<std::chrono::hours>(now - dayStart);
    
    // For simulation, calculate daily usage based on current rate
    double dailyRate = currentPowerConsumption / 1000.0; // kWh per hour
    return dailyRate * 24.0; // 24 hours
}

double EnergyMonitor::getDailyCost() const {
    return getDailyEnergyUsage() * costPerKWh;
}

double EnergyMonitor::getEfficiencyRating() const {
    return efficiencyRating;
}

double EnergyMonitor::getCarbonFootprint() const {
    return carbonFootprint;
}

double EnergyMonitor::getMoneySaved() const {
    return moneySaved;
}

std::string EnergyMonitor::getEfficiencyRecommendation() const {
    std::stringstream ss;
    
    if (efficiencyRating >= 90.0) {
        ss << "Excellent efficiency! Keep up the good work.";
    } else if (efficiencyRating >= 80.0) {
        ss << "Good efficiency. Consider optimizing temperature settings.";
    } else if (efficiencyRating >= 70.0) {
        ss << "Fair efficiency. Review schedule and temperature settings.";
    } else {
        ss << "Low efficiency. Consider energy-saving measures.";
    }
    
    return ss.str();
}

std::vector<EnergyUsage> EnergyMonitor::getUsageHistory() const {
    return usageHistory;
}

std::vector<DailyEnergyReport> EnergyMonitor::getDailyReports() const {
    return dailyReports;
}

DailyEnergyReport EnergyMonitor::getTodayReport() const {
    DailyEnergyReport report;
    report.totalEnergy = getDailyEnergyUsage();
    report.totalCost = getDailyCost();
    report.peakPower = currentPowerConsumption;
    report.averagePower = currentPowerConsumption; // Simplified for simulation
    report.runtimeHours = static_cast<int>(totalEnergyUsed / (currentPowerConsumption / 1000.0));
    report.runtimeMinutes = static_cast<int>((totalEnergyUsed / (currentPowerConsumption / 1000.0) - report.runtimeHours) * 60);
    
    // Add hourly data (simplified)
    for (int hour = 0; hour < 24; ++hour) {
        EnergyUsage hourly;
        hourly.powerConsumption = currentPowerConsumption;
        hourly.energyUsed = currentPowerConsumption / 1000.0; // 1 hour of usage
        hourly.cost = hourly.energyUsed * costPerKWh;
        hourly.timestamp = std::chrono::system_clock::now();
        report.hourlyData.push_back(hourly);
    }
    
    return report;
}

DailyEnergyReport EnergyMonitor::getYesterdayReport() const {
    // For simulation, return a simplified yesterday report
    DailyEnergyReport report;
    report.totalEnergy = getDailyEnergyUsage() * 0.8; // 80% of today's usage
    report.totalCost = report.totalEnergy * costPerKWh;
    report.peakPower = currentPowerConsumption * 0.9;
    report.averagePower = currentPowerConsumption * 0.8;
    report.runtimeHours = 8; // Typical daily runtime
    report.runtimeMinutes = 30;
    
    return report;
}

void EnergyMonitor::setCostPerKWh(double cost) {
    if (cost > 0.0) {
        costPerKWh = cost;
        calculateCost(); // Recalculate total cost
        std::cout << "Energy cost set to $" << costPerKWh << " per kWh" << std::endl;
    } else {
        std::cout << "Invalid energy cost: $" << cost << " per kWh" << std::endl;
    }
}

double EnergyMonitor::getCostPerKWh() const {
    return costPerKWh;
}

double EnergyMonitor::estimateMonthlyCost() const {
    return getDailyCost() * 30.0; // 30 days
}

double EnergyMonitor::estimateYearlyCost() const {
    return getDailyCost() * 365.0; // 365 days
}

void EnergyMonitor::resetDailyStats() {
    dayStart = std::chrono::system_clock::now();
    std::cout << "Daily energy statistics reset" << std::endl;
}

void EnergyMonitor::resetAllStats() {
    totalEnergyUsed = 0.0;
    totalCost = 0.0;
    currentPowerConsumption = 0.0;
    efficiencyRating = 85.0;
    carbonFootprint = 0.0;
    moneySaved = 0.0;
    usageHistory.clear();
    dailyReports.clear();
    dayStart = std::chrono::system_clock::now();
    std::cout << "All energy statistics reset" << std::endl;
}

void EnergyMonitor::generateDailyReport() {
    DailyEnergyReport report = getTodayReport();
    dailyReports.push_back(report);
    
    // Keep only the last MAX_DAILY_REPORTS reports
    if (dailyReports.size() > MAX_DAILY_REPORTS) {
        dailyReports.erase(dailyReports.begin());
    }
    
    std::cout << "Daily energy report generated:" << std::endl;
    std::cout << "  Total Energy: " << std::fixed << std::setprecision(2) << report.totalEnergy << " kWh" << std::endl;
    std::cout << "  Total Cost: $" << std::fixed << std::setprecision(2) << report.totalCost << std::endl;
    std::cout << "  Peak Power: " << std::fixed << std::setprecision(1) << report.peakPower << " W" << std::endl;
    std::cout << "  Runtime: " << report.runtimeHours << "h " << report.runtimeMinutes << "m" << std::endl;
}

void EnergyMonitor::cleanupOldData() {
    // Remove old usage history entries
    if (usageHistory.size() > MAX_HISTORY_SIZE) {
        size_t toRemove = usageHistory.size() - MAX_HISTORY_SIZE;
        usageHistory.erase(usageHistory.begin(), usageHistory.begin() + toRemove);
    }
    
    // Remove old daily reports
    if (dailyReports.size() > MAX_DAILY_REPORTS) {
        size_t toRemove = dailyReports.size() - MAX_DAILY_REPORTS;
        dailyReports.erase(dailyReports.begin(), dailyReports.begin() + toRemove);
    }
    
    std::cout << "Old energy data cleaned up" << std::endl;
}

void EnergyMonitor::updateEfficiencyMetrics() {
    // Calculate efficiency based on power consumption and usage patterns
    double baseEfficiency = 85.0;
    
    // Adjust efficiency based on power consumption
    if (currentPowerConsumption > 5000.0) {
        baseEfficiency -= 10.0; // Less efficient at high power
    } else if (currentPowerConsumption < 1000.0) {
        baseEfficiency += 5.0; // More efficient at low power
    }
    
    // Adjust based on usage patterns
    if (totalEnergyUsed > 50.0) {
        baseEfficiency -= 5.0; // High usage indicates less efficiency
    }
    
    efficiencyRating = std::max(0.0, std::min(100.0, baseEfficiency));
}

void EnergyMonitor::updateCarbonFootprint() {
    // Calculate carbon footprint (kg CO2)
    // Assuming 0.5 kg CO2 per kWh (typical for mixed energy sources)
    carbonFootprint = totalEnergyUsed * 0.5;
}

void EnergyMonitor::updateMoneySaved() {
    // Calculate money saved compared to a baseline
    double baselineUsage = 30.0; // 30 kWh per day baseline
    double baselineCost = baselineUsage * costPerKWh;
    double actualCost = getDailyCost();
    
    moneySaved = baselineCost - actualCost;
    if (moneySaved < 0.0) {
        moneySaved = 0.0; // No savings if over baseline
    }
}

bool EnergyMonitor::shouldGenerateDailyReport() const {
    auto now = std::chrono::system_clock::now();
    auto dayDuration = std::chrono::duration_cast<std::chrono::hours>(now - dayStart);
    
    // Generate report every 24 hours
    return dayDuration.count() >= 24;
}

void EnergyMonitor::addToHistory(const EnergyUsage& usage) {
    usageHistory.push_back(usage);
    
    // Keep history size manageable
    if (usageHistory.size() > MAX_HISTORY_SIZE) {
        usageHistory.erase(usageHistory.begin());
    }
} 