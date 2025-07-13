#pragma once

#include <random>
#include <string>
#include <chrono>

enum class WeatherCondition {
    SUNNY,
    CLOUDY,
    RAINY,
    SNOWY,
    WINDY,
    STORMY
};

struct WeatherData {
    double temperature;
    double humidity;
    double windSpeed;
    WeatherCondition condition;
    double pressure;
    std::string description;
};

class WeatherSimulator {
private:
    std::mt19937 rng;
    std::uniform_real_distribution<double> tempVariation;
    std::uniform_real_distribution<double> humidityVariation;
    std::uniform_real_distribution<double> windVariation;
    
    WeatherData currentWeather;
    WeatherData forecastWeather[24]; // 24-hour forecast
    
    std::chrono::system_clock::time_point lastUpdate;
    const int UPDATE_INTERVAL_MS = 300000; // Update every 5 minutes
    
    // Seasonal parameters
    double seasonalBaseTemp;
    double seasonalHumidity;
    WeatherCondition seasonalCondition;

public:
    WeatherSimulator();
    ~WeatherSimulator() = default;

    // Core functionality
    void initialize();
    void update();
    void shutdown();
    
    // Weather data access
    WeatherData getCurrentWeather() const;
    WeatherData getForecast(int hour) const;
    double getOutdoorTemperature() const;
    double getOutdoorHumidity() const;
    WeatherCondition getWeatherCondition() const;
    std::string getWeatherDescription() const;
    
    // Weather simulation
    void setSeasonalParameters(double baseTemp, double humidity, WeatherCondition condition);
    void simulateWeatherChange(double targetTemp, double targetHumidity, WeatherCondition condition);
    void generateForecast();
    
    // Weather impact on HVAC
    double getHeatLoad() const;
    double getCoolingLoad() const;
    bool shouldDefrost() const;
    double getOptimalIndoorTemp() const;
    
    // Weather alerts
    bool hasWeatherAlert() const;
    std::string getWeatherAlert() const;
    
    // Historical data
    void logWeatherData();
    std::string getWeatherTrend() const;

private:
    void generateRandomWeather();
    void updateForecast();
    double calculateHeatLoad() const;
    double calculateCoolingLoad() const;
    bool shouldUpdate() const;
    void applySeasonalVariation();
}; 